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

## Installation

Pre-built firmware files are available for Waveshare RP2040-Zero, Waveshare RP2350-Zero, and Waveshare ESP32-S3-Zero boards. You can find them on the [releases page](https://github.com/loopj/gcn-usb-adapter/releases/latest).

### Raspberry Pi Pico / Pico 2

Enter bootloader mode by holding the BOOTSEL button while plugging in the Pico. Then copy the firmware uf2 file to the RPI-RP2 drive that appears.

### ESP32-S3

To flash the ESP32-S3 firmware, you'll need to [install `esptool`](https://docs.espressif.com/projects/esptool/en/latest/esp32/installation.html).

Enter bootloader mode by holding the `BOOT` button, pressing and releasing the `RST` button, then releasing the `BOOT` button. The flash the firmware using `esptool` as follows:

```bash
esptool write-flash 0x0 <path_to_firmware>
```

## Wiring

- GameCube controller 5V to the board's 5V rail
- GameCube controller 3.3V to the board's 3.3V pin
- GameCube controller GND to a GND pin
- Pull-up resistors are required on each data line - the recommended value is 750Ω
- Each controller's Joybus data line to the configured GPIO

The pre-built firmwares map the four controller ports to the following GPIOs:

| Board                   | Port 1 | Port 2 | Port 3 | Port 4 |
| ----------------------- | ------ | ------ | ------ | ------ |
| Waveshare RP2040-Zero   | GP29   | GP28   | GP27   | GP26   |
| Waveshare RP2350-Zero   | GP29   | GP28   | GP27   | GP26   |
| Waveshare ESP32-S3-Zero | GP1    | GP2    | GP3    | GP4    |

## Building from Source

### Raspberry Pi Pico / Pico 2 Boards

Install the [Pico SDK](https://github.com/raspberrypi/pico-sdk) and set the `PICO_SDK_PATH` environment variable to the path of your installation.

You can build for one of the pre-configured boards using the `cmake` presets as follows:

```bash
cd ports/rp2xxx
cmake --preset rp2040-zero
cmake --build --preset rp2040-zero
```

Alternatively, you can do a custom build by passing your own configuration options to cmake directly:

```bash
cmake -B build \
  -DPICO_BOARD=pico \
  -DGCCA_JOYBUS_GPIO_CH0=1 \
  -DGCCA_JOYBUS_GPIO_CH1=2 \
  -DGCCA_JOYBUS_GPIO_CH2=3 \
  -DGCCA_JOYBUS_GPIO_CH3=4
```

### ESP32-S3 Boards

Install the [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) toolchain.

You can build for one of the pre-configured boards by specifying the defaults file for that board as follows:

```bash
cd ports/esp32
idf.py set-target esp32s3
idf.py -D SDKCONFIG_DEFAULTS="sdkconfig.defaults;boards/esp32-s3-zero.defaults" build
```

For a custom build, you can configure the port GPIOs under the **GameCube Controller Adapter** inside `idf.py menuconfig`, and then build with `idf.py build`.
