#!/home/adam/Documents/Programming/C++/FIT/IMP/heartbeat_sensor/venv/bin/python3
"""
Heartbeat Sensor Data Plotter
Plots timestamp vs signal data from CSV files in the data directory and saves figures
"""

import os
import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import glob
from pathlib import Path

def plot_sensor_data(data_dir="data"):
    """
    Plot timestamp vs signal data from CSV files in the data directory and save figures

    Args:
        data_dir (str): Directory containing CSV files (default: "data")
    """
    # Get all CSV files in the data directory
    csv_files = glob.glob(os.path.join(data_dir, "*.csv"))

    if not csv_files:
        print(f"No CSV files found in {data_dir} directory")
        return

    print(f"Found {len(csv_files)} CSV file(s):")
    for file in csv_files:
        print(f"  - {os.path.basename(file)}")

    # Check if this is measurements directory - create separate figures
    is_measurements = "measurements" in data_dir

    if is_measurements:
        # Create separate figures for each measurement
        for i, csv_file in enumerate(csv_files):
            plot_single_file(csv_file, i)
    else:
        # Create separate figures for examples as well
        for i, csv_file in enumerate(csv_files):
            plot_single_file(csv_file, i)


def plot_single_file(csv_file, index):
    """Plot a single CSV file with signal and BPM data in separate subplots"""
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10), sharex=True)
    
    # Plot signal data on top subplot
    plot_signal_on_axis(csv_file, ax1)
    
    # Plot BPM data on bottom subplot
    plot_bpm_on_axis(csv_file, ax2)
    
    # Set overall title
    fig.suptitle(f'Heartbeat Sensor Data - {os.path.basename(csv_file)}', fontsize=14)
    
    # Adjust layout
    plt.tight_layout()
    fig.subplots_adjust(top=0.92)

    # Save the figure next to the data file
    csv_path = Path(csv_file)
    output_path = csv_path.with_suffix('.png')
    fig.savefig(output_path, dpi=300, bbox_inches='tight')
    print(f"Saved figure to: {output_path}")

    plt.close(fig)  # Close the figure to free memory


def plot_signal_on_axis(csv_file, ax):
    """Plot signal data on the given axis"""
    try:
        # Read first few lines to check if file has headers
        with open(csv_file, 'r') as f:
            first_line = f.readline().strip()
            # Check if first line contains numeric values (likely data) or text (likely headers)
            try:
                # If first value is numeric, assume no headers
                float(first_line.split(',')[0])
                has_headers = False
            except ValueError:
                # If first value is not numeric, assume headers
                has_headers = True

        if has_headers:
            df = pd.read_csv(csv_file)
            timestamp_col = 'timestamp'
            signal_col = 'signal'
            threshold_col = 'threshold'
        else:
            df = pd.read_csv(csv_file, header=None,
                           names=['timestamp', 'signal', 'peak', 'trough', 'threshold', 'beat_detected', 'bpm'])
            timestamp_col = 'timestamp'
            signal_col = 'signal'
            threshold_col = 'threshold'

        # Plot the data
        time_offset = df[timestamp_col].min()
        ax.plot(df[timestamp_col] - time_offset, df[signal_col], 'b-', linewidth=1, alpha=0.8, label='Signal')
        ax.set_ylabel('Signal Value')
        ax.set_title('Signal Data')
        ax.grid(True, alpha=0.3)

        # Plot threshold if available
        if threshold_col in df.columns:
            ax.plot(df[timestamp_col] - time_offset, df[threshold_col], color='purple', linewidth=1.5,
                   label='Threshold', alpha=0.8)

        # Plot peak and trough if available
        if 'peak' in df.columns:
            ax.plot(df[timestamp_col] - time_offset, df['peak'], color='green', linewidth=1,
                   label='Peak', alpha=0.8)
        if 'trough' in df.columns:
            ax.plot(df[timestamp_col] - time_offset, df['trough'], color='orange', linewidth=1,
                   label='Trough', alpha=0.8)

        # Plot beat detected markers
        if 'beat_detected' in df.columns:
            beats = df[df['beat_detected'] == 1]
            if not beats.empty:
                ax.scatter(beats[timestamp_col] - time_offset, beats[signal_col], color='red', s=30,
                          label='Beats Detected', alpha=0.9)

        ax.legend()

    except Exception as e:
        print(f"Error processing signal data from {csv_file}: {e}")


def plot_bpm_on_axis(csv_file, ax):
    """Plot BPM data on the given axis"""
    try:
        # Read first few lines to check if file has headers
        with open(csv_file, 'r') as f:
            first_line = f.readline().strip()
            # Check if first line contains numeric values (likely data) or text (likely headers)
            try:
                # If first value is numeric, assume no headers
                float(first_line.split(',')[0])
                has_headers = False
            except ValueError:
                # If first value is not numeric, assume headers
                has_headers = True

        if has_headers:
            df = pd.read_csv(csv_file)
            timestamp_col = 'timestamp'
            bpm_col = 'bpm'
            beat_detected_col = 'beat_detected'
        else:
            df = pd.read_csv(csv_file, header=None,
                           names=['timestamp', 'signal', 'peak', 'trough', 'threshold', 'beat_detected', 'bpm'])
            timestamp_col = 'timestamp'
            bpm_col = 'bpm'
            beat_detected_col = 'beat_detected'

        # Count total beats detected
        total_beats = df[beat_detected_col].sum()

        # Calculate sliding average BPM (last 5 non-zero values from data)
        valid_bpm_values = df[df[bpm_col] > 0][bpm_col]
        if len(valid_bpm_values) >= 5:
            sliding_avg_bpm = valid_bpm_values.tail(5).mean()
        elif len(valid_bpm_values) > 0:
            sliding_avg_bpm = valid_bpm_values.mean()
        else:
            sliding_avg_bpm = 0

        # Calculate BPM from beat detections with sliding average
        beat_timestamps = df[df[beat_detected_col] == 1][timestamp_col].values
        beat_based_bpms = []
        
        if len(beat_timestamps) > 1:
            # Calculate BPM between consecutive beats
            intervals_ms = np.diff(beat_timestamps)  # Time differences in milliseconds
            intervals_sec = intervals_ms / 1000.0    # Convert to seconds
            bpm_values = 60.0 / intervals_sec         # BPM = 60 / interval in seconds
            
            # Apply sliding average of last 5 BPM values
            if len(bpm_values) >= 5:
                for i in range(len(bpm_values)):
                    start_idx = max(0, i - 4)  # Include up to 5 previous values
                    avg_bpm = np.mean(bpm_values[start_idx:i+1])
                    beat_based_bpms.append(avg_bpm)
            else:
                # If less than 5 beats, use all available
                beat_based_bpms = bpm_values.tolist()
            
            # Create time array for beat-based BPMs (use beat timestamps)
            beat_times = beat_timestamps[1:len(beat_based_bpms)+1]  # Skip first beat, align with BPMs
        else:
            beat_times = []
            beat_based_bpms = []

        # Plot BPM data from sensor
        time_offset = df[timestamp_col].min()
        ax.plot(df[timestamp_col] - time_offset, df[bpm_col], 'r-', linewidth=2, alpha=0.8, label='Sensor BPM')
        
        # Plot BPM calculated from beats
        if len(beat_times) > 0 and len(beat_based_bpms) > 0:
            ax.plot(beat_times - time_offset, beat_based_bpms, 'b--', linewidth=2, alpha=0.8, label='Beat-based BPM')
        
        ax.set_xlabel('Time (ms from start)')
        ax.set_ylabel('BPM')
        
        # Update title with beat count and sliding average BPM
        title = f'Heart Rate (BPM)\nBeats: {int(total_beats)}, Sensor Avg: {sliding_avg_bpm:.1f}'
        if len(beat_based_bpms) > 0:
            title += f', Beat Avg: {np.mean(beat_based_bpms):.1f}'
        ax.set_title(title)
        
        ax.grid(True, alpha=0.3)
        ax.legend()

        # Set Y limits for BPM starting from zero
        valid_bpm = df[df[bpm_col] > 0][bpm_col]
        if not valid_bpm.empty:
            bpm_max = min(200, valid_bpm.max() + 10)
            ax.set_ylim(bottom=0, top=bpm_max)
        else:
            ax.set_ylim(bottom=0, top=200)

    except Exception as e:
        print(f"Error processing BPM data from {csv_file}: {e}")

def main():
    # Allow custom data directory as command line argument
    data_dir = sys.argv[1] if len(sys.argv) > 1 else "data"

    # Check if data directory exists
    if not os.path.exists(data_dir):
        print(f"Error: Data directory '{data_dir}' not found!")
        print("Usage: python plot_sensor_data.py [data_directory]")
        sys.exit(1)

    plot_sensor_data(data_dir)

if __name__ == "__main__":
    main()