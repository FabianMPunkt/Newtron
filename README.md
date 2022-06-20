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
- Perfboard (dimensions here)
- generic push button
- JST-XH Connectors
- 3D Printed parts


Hardware is still very much WIP.
The Hardware and assembly is made to be as simple as possible, and with as few printed parts as possible.

my plan is it to make the screen and the button all one "front panel" unit, and in the back having the teensy soldered to a piece of perfboard, srewed to the frame.
The screen & the USB connector will be connected to the teensy via a JST XH connector. everything clearly labeled on the inside.


TODO:

- make 3d-printalbe encosure
- make build log / instructions
- add i2c-LCD to Teensy code
- make debug commands via serial
- build the damn thing
