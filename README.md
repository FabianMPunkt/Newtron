# Newtron



This little project is made to read the USBSerial output of one these ["TesT"-devices](https://www.test-gmbh.com/de/produkte/pruefmaschinen/einfache-pruefrahmen/modell-105/).
It will process the readings, turn them all into positive values and save the highest value.

With the press of a button the highest value will the be sent to the computer using the "Keyboard.h" library.


The TesT-device trannsmits all the values over its USBSerial Port in a single line as seen in [these readings](misc/OG_readings.txt).
GitHub's formatting doen't disply this correctly though. All the values are in-fact separated by a ""

Now you may be wondering how we can read this data using an Arduino, as the "TesT" only has a USB-Interface.
After searching through virtually every existing Arduino forum there is, i finally found a solution:


## Arduino:

At first i was using one of these little [USB Host Boards](https://www.hobbytronics.co.uk/usb-host/usb-host-board-v24), sold by HobbyTronics and an Arduino Leonardo.
These Host boards can, among many other cool things, take data from a USBSerial connection, and "convert" it to standard RX / TX signals.
After that, reading the Serial Port using the Arduino would be very simple.
All that code is still present.
(files labeled with "Leonardo")


## Teensy:

Then i figured out that all this would be much simpler using a Teensy 4.1, as it has a USB Host built in.
It's essentially the same, only with some extra USB code that i ripped from [here](https://github.com/PaulStoffregen/USBHost_t36/blob/master/examples/Serial/Serial.ino).
(files labeled with "Teensy")


## Hardware:

Parts list:
- [Teensy 4.1](https://www.conrad.de/de/p/pjrc-mikrocontroller-teensy-4-1-2269230.html)
- [16x2 I2C-LCD Display](https://www.conrad.de/de/p/joy-it-sbc-lcd16x2-display-modul-6-6-cm-2-6-zoll-16-x-2-pixel-passend-fuer-entwicklungskits-raspberry-pi-arduino-b-1503825.html)
- [Through-hole USB female connector](https://de.rs-online.com/web/p/usb-steckverbinder/6741325)
- [3.5mm socket](https://de.rs-online.com/web/p/klinken-steckerbuchsen/5051429)
- generic push button
- JST-XH Connectors
- [3D Printed parts](CAD/3MF/)
- M4x20 Screws


I tried making the enclosure as simple as possible, but also have it not be an ugly box.
There's three parts to print: [Base](CAD/3MF/Newtron_Base.3mf), [Top](CAD/3MF/Newtron_Top.3mf) and [Front](CAD/3MF/Newtron_Front.3mf)



### Build guide:

WIP

First up, here's all the connector cables you'll need to make:

USB Port:
![20220804_093035](https://user-images.githubusercontent.com/78741736/182795334-7a1e6e6e-6357-480d-b735-0dfa330438a1.JPG)

Pedal jack:
![20220804_093047](https://user-images.githubusercontent.com/78741736/182795389-f8a6395b-4bba-4732-a6f8-5188ec67dc37.JPG)

Button:
![20220804_093101](https://user-images.githubusercontent.com/78741736/182795409-15501130-b6f0-4c5a-8f1e-994f052e65b4.JPG)

Display cables
![20220804_093246](https://user-images.githubusercontent.com/78741736/182795470-eda12e60-476b-4890-826c-eccdea96f23d.JPG)


I just soldered some JST-XH connectors directly to the Teensy like this:
![Teensy](https://user-images.githubusercontent.com/78741736/182794769-4b2c661b-feb1-41eb-9864-1852cbda7a34.JPG)

Refer to the [Schematic](CAD/Newtron_Schematic.pdf) for more detailed wiring.
