#!/usr/bin/env python3
"""
Automatic data saver - listens to serial port and saves data when recording stops.
Usage: python3 scripts/auto_save_listener.py [port]

This script runs in the background and automatically saves data to the data/
directory whenever the ESP32 stops recording.
"""

import serial
import sys
import os
from datetime import datetime
import time

def listen_and_save(port='/dev/ttyUSB1', baudrate=115200):
    """Listen to serial port and automatically save data when received."""
    
    # Create data directory if it doesn't exist
    data_dir = './data'
    os.makedirs(data_dir, exist_ok=True)
    
    try:
        print(f"Connecting to ESP32 on port {port}...")
        ser = serial.Serial(port, baudrate, timeout=1)
        
        # Wait for connection to stabilize
        time.sleep(1)
        
        print("✓ Connected! Listening for data...")
        print("Press Ctrl+C to stop\n")
        
        recording_data = False
        data_lines = []
        
        while True:
            try:
                if ser.in_waiting > 0:
                    line = ser.readline().decode('utf-8', errors='ignore').strip()
                    
                    # Echo important messages
                    if 'Recording started' in line or 'Recording stopped' in line:
                        print(f"[ESP32] {line}")
                    
                    # Detect data dump markers
                    if line == "===DATA_START===":
                        recording_data = True
                        data_lines = []
                        print("→ Receiving data...")
                        continue
                    elif line == "===DATA_END===":
                        recording_data = False
                        
                        if data_lines:
                            # Generate output filename with timestamp
                            timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
                            output_file = os.path.join(data_dir, f'sensor_data_{timestamp}.csv')
                            
                            # Write data to file
                            with open(output_file, 'w') as f:
                                for data_line in data_lines:
                                    f.write(data_line + '\n')
                            
                            print(f"✓ Data saved to: {output_file}")
                            print(f"  Lines: {len(data_lines)}")
                            print(f"  First line: {data_lines[0] if data_lines else 'N/A'}")
                            print()
                        else:
                            print("⚠ No data received")
                        
                        data_lines = []
                        continue
                    
                    # Collect data lines
                    if recording_data and line:
                        data_lines.append(line)
                
            except UnicodeDecodeError:
                # Skip lines with encoding issues
                pass
            
            time.sleep(0.01)  # Small delay to prevent CPU spinning
            
    except serial.SerialException as e:
        print(f"\nERROR: Could not open serial port {port}")
        print(f"Details: {e}")
        print("\nAvailable ports:")
        try:
            from serial.tools import list_ports
            for p in list_ports.comports():
                print(f"  {p.device}")
        except:
            pass
        return False
    except KeyboardInterrupt:
        print("\n\n✓ Listener stopped by user")
        ser.close()
        return True
    except Exception as e:
        print(f"\nERROR: {e}")
        return False

if __name__ == '__main__':
    port = sys.argv[1] if len(sys.argv) > 1 else '/dev/ttyUSB1'
    
    print("=" * 60)
    print("ESP32 Heartbeat Sensor - Automatic Data Saver")
    print("=" * 60)
    
    success = listen_and_save(port)
    sys.exit(0 if success else 1)
