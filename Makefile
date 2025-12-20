PIO = /home/adam/.platformio/penv/bin/platformio

all: flash monitor

flash: build upload

build:
	$(PIO) run

upload:
	$(PIO) run -t upload

monitor:
	$(PIO) device monitor -b 115200

clean:
	$(PIO) run -t clean
	rm -f data/**/*.png

venv:
	python3 -m venv venv
	./venv/bin/pip install pandas matplotlib

plot:
	./scripts/plot_sensor_data.py data/measurements
	./scripts/plot_sensor_data.py data/examples

.PHONY: all build upload flash monitor clean venv plot
