# USB Adapter for GameCube Controllers

Open-source 4-port USB GameCube Controller Adapter, a drop-in replacement for
Nintendo's official adapter (WUP-028).

Just like the official adapter, this reads up to four GameCube controllers over the Joybus protocol and exposes them to a USB host over its own custom HID interface.

Built using [`libjoybus`](https://github.com/loopj/libjoybus) and [TinyUSB](https://github.com/hathach/tinyusb). Implementations are provided for ESP32-S3 and Raspberry Pi Pico (RP2040/RP2350).

## Features

- Up to 4 GameCube controllers
- Rumble support
- WaveBird receiver support (unlike the Mayflash clones)
- 1 kHz polling end to end for low input latency
- Recognized as the official Nintendo adapter on Wii U, Switch, Switch 2, and Dolphin

## Wiring

- GameCube controller 5V to the board's 5V rail
- GameCube controller 3.3V to the board's 3.3V pin
- GameCube controller GND to a GND pin
- Connect each Joybus data line to the GPIO pin listed in the platform's `main.c` (`JOYBUS_GPIOS`)

Pull-up resistors are required on each data line; the recommended value is 750Ω.
