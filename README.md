# Floower

The most advanced flower in the world.

https://floower.io

![Everblooming Flowers]()

## Features

- 6 petals that open and close - operated by servo motor inside the flowerpot
- 7 RGS LEDs in the center of the blossom
- capacitive touch sensor in the leaf
- ESP32 microcontroller
- Arduino IDE or MicroPython support
- Bluetooth & WiFi connectivity
- USB-C interface for programming (CP2102N USB-to-UART)
- 1600 mAh LIPO battery (TP4056 charging IC)
- deep sleep mode to extend battery life
- red charging and blue activity LEDs in the flowerpot

## How do I get one?

...

## Understanding Floower hardware

...

### Logic Boards History

- **rev5** - 
- **rev5.5** (marked as A) - 
- **rev6** - 

## Connecting flower to the computer

### Installing 

Floower can be programmed using the Arduino IDE. However, the ESP32 microcontroller is not supported by default, so you need to install & configure it:

- Prerequisites: Arduino IDE Installed
- Install [CP2102 Universal Driver](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers)
- Install ESP32 Add-on in Arduino IDE
- Open **File > Preferences**
- Enter **https://dl.espressif.com/dl/package_esp32_index.json** into the **Additional Board Manager URLs** field and click **OK** button. (If you already have other boards URL, separate the URLs with a comma)

![Additional Board Manager URLs]()

- Open **Tools > Boards > Boards Manager...** and search for esp32
- Click Install button for the **ESP32 by Espressif Systems**. It should be installed after a few seconds.

![Installing ESP32 by Espressif Systems]()

- Open **Tools > Boards** and select **DOIT ESP32 DEVKIT V1**
- That's it!

![Selecting DOIT ESP32 DEVKIT V1 board]()

### First upload

Checkout this repository and open [flower-esp32.ino](src/flower-esp32/flower-esp32.ino) project file. Connect your Floower to your computer via USB-C data cable. Make sure you are using smart/data USB cable, some of the cables are power only and thus cannot transfer data at all. Select the port of your Floower, it should be the one that just appeared.

Run **Sketch > Upload**. You should get a sweet *'avrdude done. Thank you.'* message at the end of the console. Now you can make your own personalized Floower.

![Arduino IDE upload](https://raw.githubusercontent.com/jpraus/arduinoflake/master/doc/arduinoide-upload.png)
