# RadioKnobs

RadioKnobs is a small project for [Arduino](https://www.arduino.cc/) that takes input from rotary encoders and outputs serial commands via USB. It's intended to use along with a Raspberry Pi running [HiFiBerryOS](https://www.hifiberry.com/hifiberryos/).

## Setup

This project was written in VSCode using [PlatformIO](https://platformio.org/). After adding the project to PlatformIO, use the `pio lib install` command to install library dependencies.

The project was built using an [Adafruit Metro Mini 328](https://www.adafruit.com/product/2590) but should work with a number of other Arduino-based microcontrollers. If you're not using a Metro Mini you should make sure to modify `platformio.ini` to reflect the environment you're using.
