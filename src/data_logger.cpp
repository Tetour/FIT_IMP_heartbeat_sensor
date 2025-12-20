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

  // Create file and write CSV header
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (file) {
    file.println("timestamp,signal,peak,trough,threshold,beat_detected,bpm");
    file.close();

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
  if (!recordingEnabled) {
    return;
  }

  // Use the recording filename or default
  const char* actualFilename = recordingFilename.length() > 0 ?
                              recordingFilename.c_str() : "/sensor_data.csv";

  File file = SPIFFS.open(actualFilename, FILE_APPEND);
  if (!file) {
    // Only report error once per session to avoid console spam
    static bool errorReported = false;
    if (!errorReported && Serial) {
      Serial.print("Failed to open file: ");
      Serial.println(actualFilename);
      errorReported = true;
    }
    return;
  }

  // Write relevant data in CSV format
  file.print(timestamp);
  file.print(",");
  file.print(signal);
  file.print(",");
  file.print(peak);
  file.print(",");
  file.print(trough);
  file.print(",");
  file.print(threshold);
  file.print(",");
  file.print(beatDetected ? "1" : "0");
  file.print(",");
  file.println(bpm);

  file.close();
}