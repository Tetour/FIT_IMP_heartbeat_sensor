#!/home/adam/Documents/Programming/C++/FIT/IMP/heartbeat_sensor/venv/bin/python3
"""
Heartbeat Sensor Data Plotter
Plots timestamp vs signal data from CSV files in the data directory and saves figures
"""

import os
import sys
import pandas as pd
import matplotlib.pyplot as plt
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
        # Create subplots for multiple files (original behavior)
        fig, axes = plt.subplots(len(csv_files), 1, figsize=(12, 6 * len(csv_files)))
        if len(csv_files) == 1:
            axes = [axes]  # Make it iterable for single plot

        for i, csv_file in enumerate(csv_files):
            plot_file_on_axis(csv_file, axes[i])

        plt.tight_layout()

        # Save the combined figure
        data_path = Path(data_dir)
        if len(csv_files) == 1:
            # Single file - save with same name as CSV
            csv_path = Path(csv_files[0])
            output_path = csv_path.with_suffix('.png')
        else:
            # Multiple files - save as combined plot in data directory
            output_path = data_path / "combined_plots.png"

        fig.savefig(output_path, dpi=300, bbox_inches='tight')
        print(f"Saved combined figure to: {output_path}")

        plt.close(fig)  # Close the figure to free memory


def plot_single_file(csv_file, index):
    """Plot a single CSV file in its own figure"""
    fig, ax = plt.subplots(1, 1, figsize=(12, 6))
    plot_file_on_axis(csv_file, ax)

    # Save the figure next to the data file
    csv_path = Path(csv_file)
    output_path = csv_path.with_suffix('.png')
    fig.savefig(output_path, dpi=300, bbox_inches='tight')
    print(f"Saved figure to: {output_path}")

    plt.close(fig)  # Close the figure to free memory


def plot_file_on_axis(csv_file, ax):
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
        ax.plot(df[timestamp_col] - time_offset, df[signal_col], 'b-', linewidth=1, alpha=0.8)
        ax.set_xlabel('Time (ms from start)')
        ax.set_ylabel('Signal Value')
        ax.set_title(f'Heartbeat Sensor Signal - {os.path.basename(csv_file)}')
        ax.grid(True, alpha=0.3)

        # Plot threshold if available
        if threshold_col in df.columns:
            ax.plot(df[timestamp_col] - time_offset, df[threshold_col], color='purple', linewidth=1.5,
                   label='Threshold', alpha=0.8)

        # Plot peak and trough if available
        if 'peak' in df.columns:
            ax.plot(df[timestamp_col] - time_offset, df['peak'], color='red', linewidth=1,
                   label='Peak', alpha=0.8)
        if 'trough' in df.columns:
            ax.plot(df[timestamp_col] - time_offset, df['trough'], color='blue', linewidth=1,
                   label='Trough', alpha=0.8)

        # Plot beat detected markers
        if 'beat_detected' in df.columns:
            beats = df[df['beat_detected'] == 1]
            if not beats.empty:
                ax.scatter(beats[timestamp_col] - time_offset, beats[signal_col], color='green', s=10,
                          label='Beats Detected', alpha=0.8)

        ax.legend()

        print(f"Plotted {len(df)} data points from {os.path.basename(csv_file)}")
    except Exception as e:
        print(f"Error processing {csv_file}: {e}")

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