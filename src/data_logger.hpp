#pragma once

#include <Arduino.h>
#include <SPIFFS.h>

class DataLogger {
private:
    bool recordingEnabled;
    String recordingFilename;
    File recordingFile;  // Keep file handle open during recording
    bool debugOutput;    // Debug output control
    int autoRecordingTime;  // Autorecording duration in seconds
    unsigned long recordingStartTime;  // Timestamp when recording started

    // Configuration defaults and limits
    static const int DEFAULT_AUTO_RECORDING_TIME = 30;
    static const int AUTO_RECORDING_MIN = 0;
    static const int AUTO_RECORDING_MAX = 300;

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

    // Autorecording configuration
    void setAutoRecordingTime(int time);
    int getAutoRecordingTime() const;
    unsigned long getRecordingStartTime() const;
    void checkAutoStop();  // Check if autorecording time has elapsed and stop if needed

    // Configuration limits
    static int getAutoRecordingMin() { return AUTO_RECORDING_MIN; }
    static int getAutoRecordingMax() { return AUTO_RECORDING_MAX; }

    // Debug output control
    void setDebugOutput(bool enable);
    bool getDebugOutput() const;
};