PIO = /home/adam/.platformio/penv/bin/platformio

all: flash autosave

flash: build upload

build:
	$(PIO) run

upload:
	$(PIO) run -t upload

monitor:
	$(PIO) device monitor -b 115200

autosave:
	python3 scripts/auto_save_listener.py

plot: plot-clean
	./scripts/plot_sensor_data.py data/measurements
	./scripts/plot_sensor_data.py data/examples

plot-clean:
	rm -f data/**/*.png

venv:
	python3 -m venv venv
	./venv/bin/pip install pandas matplotlib

clean: plot-clean
	$(PIO) run -t clean

.PHONY: all build upload flash monitor clean venv plot autosave
