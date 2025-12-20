#include "data_logger.hpp"
#include <SPIFFS.h>

DataLogger::DataLogger() :
    recordingEnabled(false) {
}

void DataLogger::init() {
  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    if (Serial) {
      Serial.println("SPIFFS initialization failed!");
    }
  } else {
    if (Serial) {
      Serial.println("SPIFFS initialized successfully");
    }
  }
}

void DataLogger::startRecording(const char* filename) {
  recordingFilename = filename;

  // Delete existing file and create new one with CSV header
  if (SPIFFS.exists(filename)) {
    SPIFFS.remove(filename);
    if (Serial) {
      Serial.println("Cleared existing data file");
    }
  }

  // Open file once and keep it open for the entire recording session
  recordingFile = SPIFFS.open(filename, FILE_WRITE);
  if (recordingFile) {
    recordingFile.println("timestamp,signal,peak,trough,threshold,beat_detected,bpm");
    recordingEnabled = true;

    if (Serial) {
      Serial.print("Started recording data to: ");
      Serial.println(filename);
      Serial.println("NOTE: Data is saved to ESP32 SPIFFS filesystem");
    }
  } else {
    if (Serial) {
      Serial.print("Failed to create recording file: ");
      Serial.println(filename);
    }
  }
}

void DataLogger::stopRecording() {
  recordingEnabled = false;

  // Close the recording file
  if (recordingFile) {
    recordingFile.close();
  }

  if (Serial) {
    Serial.println("Stopped recording data");
    Serial.println("Auto-dumping data...");
  }

  // Automatically dump recorded data when stopping
  dumpRecordedData();
}

bool DataLogger::isRecording() const {
  return recordingEnabled;
}

void DataLogger::dumpRecordedData() {
  if (!recordingFilename.length()) {
    if (Serial) {
      Serial.println("ERROR: No recording filename set");
    }
    return;
  }

  File file = SPIFFS.open(recordingFilename.c_str(), FILE_READ);
  if (!file) {
    if (Serial) {
      Serial.print("ERROR: Failed to open file for reading: ");
      Serial.println(recordingFilename);
    }
    return;
  }

  if (Serial) {
    Serial.println("===DATA_START===");
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');
    if (Serial) {
      Serial.println(line);
    }
  }

  file.close();

  if (Serial) {
    Serial.println("===DATA_END===");
  }
}

void DataLogger::logData(unsigned long timestamp, int signal, int peak, int trough,
                        int threshold, bool beatDetected, int bpm) {
  if (!recordingEnabled || !recordingFile) {
    return;
  }

  // Write data to the already open file
  recordingFile.print(timestamp);
  recordingFile.print(",");
  recordingFile.print(signal);
  recordingFile.print(",");
  recordingFile.print(peak);
  recordingFile.print(",");
  recordingFile.print(trough);
  recordingFile.print(",");
  recordingFile.print(threshold);
  recordingFile.print(",");
  recordingFile.print(beatDetected ? "1" : "0");
  recordingFile.print(",");
  recordingFile.println(bpm);

  // Periodic flush for data integrity (every 100 writes)
  static int writeCount = 0;
  if (++writeCount % 100 == 0) {
    recordingFile.flush();
  }
}