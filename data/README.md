# Data Directory

This directory stores sensor data downloaded from the ESP32.

## Files

Data files are automatically named with timestamps:
- Format: `sensor_data_YYYYMMDD_HHMMSS.csv`
- Example: `sensor_data_20251220_143052.csv`

## CSV Format

Each data file contains:
```csv
timestamp,signal,peak,trough,bpm,beat_detected
1234,2048,3000,1500,72,0
1254,2100,3000,1500,72,1
...
```

### Columns:
- **timestamp**: Milliseconds since ESP32 boot
- **signal**: Raw sensor reading (0-4095)
- **peak**: Current peak value for threshold calculation
- **trough**: Current trough value for threshold calculation
- **bpm**: Calculated beats per minute (smoothed over 100 beats)
- **beat_detected**: 1 if beat detected in this sample, 0 otherwise

## Downloading Data

### Method 1: Automatic (Recommended)

Run the auto-save listener script in the background. It will automatically save data to this directory whenever you stop recording on the ESP32:

```bash
python3 scripts/auto_save_listener.py [port]
```

The script will:
- Connect to the ESP32
- Listen for recording events
- Automatically save data when you press RIGHT joystick (stop recording)
- Continue running until you press Ctrl+C

### Method 2: Manual

Use the download script to manually fetch data on demand:

**Python:**
```bash
python3 scripts/download_data.py [port]
```

**Bash:**
```bash
./scripts/download_data.sh [port]
```

Default port is `/dev/ttyUSB1`. Specify a different port if needed:
```bash
python3 scripts/auto_save_listener.py /dev/ttyUSB0
```

## Workflow

### Automatic Mode (Recommended):
1. Start listener: `python3 scripts/auto_save_listener.py`
2. Press **LEFT joystick** on ESP32 main screen to start recording
3. Record data for desired duration
4. Press **RIGHT joystick** to stop recording
5. Data automatically saved to `data/sensor_data_YYYYMMDD_HHMMSS.csv`
6. Repeat steps 2-5 as needed
7. Press Ctrl+C to stop listener

### Manual Mode:
1. Press **LEFT joystick** on ESP32 main screen to start recording
2. Record data for desired duration
3. Press **RIGHT joystick** to stop recording
4. Run: `python3 scripts/download_data.py`
5. Data saved to this directory with timestamp
