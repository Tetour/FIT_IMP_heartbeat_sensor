#pragma once

#include <Arduino.h>
#include <SPIFFS.h>

class DataLogger {
private:
    bool recordingEnabled;
    String recordingFilename;
    File recordingFile;  // Keep file handle open during recording

public:
    DataLogger();
    void init();

    // Data recording control
    void startRecording(const char* filename = "/sensor_data.csv");
    void stopRecording();
    bool isRecording() const;
    void dumpRecordedData();

    // Data logging - called from sensor with data
    void logData(unsigned long timestamp, int signal, int peak, int trough,
                 int threshold, bool beatDetected, int bpm);
};