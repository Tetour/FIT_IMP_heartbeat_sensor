# ESP32 Heartbeat Sensor

A comprehensive heartbeat monitoring system built on ESP32 with OLED display, joystick control, and automatic data logging capabilities.

## Features

- **Real-time Heartbeat Detection**: Pulse sensor with adaptive thresholding
- **Configurable Parameters**: Adjustable threshold, BPM offset, and decay rate
- **OLED Display**: 128x64 SSD1306 display showing BPM and navigation menus
- **Joystick Control**: 5-button joystick for menu navigation and recording control
- **Data Logging and Visualization**: Automatic CSV logging to ESP32 SPIFFS filesystem
- **Auto Data Export**: Python script for automatic data retrieval and saving

## Hardware Requirements

- **ESP32 Board**: WEMOS D1 R32 or compatible ESP32
- **Pulse Sensor**: Analog heartbeat sensor 
- **OLED Display**: 128x64 SSD1306 I2C display
- **Joystick**: 5-button digital joystick module

## Software Requirements

- **PlatformIO**: IDE and build system for embedded development
  - Install PlatformIO Core

## Quick Start

### 1. Hardware Setup

Connect the components using these pin assignments:

| Component        | Function      | ESP32 Pin     |
|------------------|---------------|---------------|
| **Pulse Sensor** | Analog signal | GPIO 34       |
| **OLED Display** | I2C SDA       | GPIO 21 (SDA) |
|                  | I2C SCL       | GPIO 22 (SCL) |
| **Joystick**     | UP input      | GPIO 18       |
|                  | DOWN input    | GPIO 19       |
|                  | LEFT input    | GPIO 23       |
|                  | RIGHT input   | GPIO 5        |
|                  | PRESS input   | GPIO 13       |

All components are powered from the ESP32's 3.3V output.

### 2. Software Setup
Install PlatformIO either by: which you can isntall eiher 
  - by installing VS Code PlatformIO extension
  - or running the following command:
  ```bash
  pip install platformio
  ```

### 2. Build and Upload
```bash
make build
make upload
```

### 3. Monitor Output
```bash
make monitor
```

### 4. Measurements and Data Recording
Use the joystick to navigate menus, configure parameters and start recording.

### 5. Generate Plots from Recorded Data
```bash
make 
```

## Usage Guide

### Menu Navigation
- **Middle Button**: Toggle between BPM display and settings menu
- **Up/Down**: Navigate menu items
- **Left/Right**: Adjust values in settings menu

### Recording Control
- **Left/Right Buttons** (in BPM mode): Start/stop recording
- Data is automatically saved to ESP32 SPIFFS and exported via serial

### Serial Commands
- `HELP`: Show available commands
- `DUMP`: Manually dump recorded data to serial

## Configuration

### Sensor Parameters (in sensor.hpp)
- `DEFAULT_THRESHOLD_OFFSET`: Base threshold adjustment
- `DEFAULT_PEAK_DECAY_RATE`: Peak signal decay speed
- `DEFAULT_TROUGH_DECAY_RATE`: Trough signal decay speed
- `DEFAULT_BPM_OFFSET`: BPM calculation offset

### Joystick Parameters (in joystick.hpp)
- `DEBOUNCE_DELAY`: Button debounce time (50ms)
- Debug output control via `setDebugOutput(bool)`

## Data Format

### CSV Structure
```csv
timestamp,signal,peak,trough,threshold,beat_detected,bpm
1234567,2048,3000,1500,2250,1,72
```

### Data Collection
- **Sample Rate**: 50Hz (every 20ms)
- **Flush Frequency**: Every 100 data points for data integrity
- **Auto-Export**: Triggered when recording stops

## Build System

### PlatformIO Targets
- `make build`: Compile the firmware
- `make upload`: Upload to ESP32
- `make monitor`: Open serial monitor
- `make flash`: Build and upload
- `make clean`: Clean build files

### Custom Targets
- `make autosave`: Run automatic data saver
- `make plot`: Generate data plots
- `make venv`: Create Python virtual environment

## Troubleshooting

### Debug Options

Each class has a `setDebugOutput(bool)` method for enabling debug output.

## License

This project uses WTFPL license. See [LICENSE](LICENSE) file for details.

## Authors

Adam Vrška