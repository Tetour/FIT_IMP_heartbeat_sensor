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

.PHONY: all build upload flash monitor clean
