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
- Teensy 4.1
- 16x2 LCD display (I2C)
- Perfboard (dimensions here)
- generic push button
- Through-hole USB female connector
- 3.5mm socket
- 3D Printed parts

Hardware is still very much WIP.
The Hardware and assembly is made to be as simple as possible, and with as few printed parts as possible.

my plan is it ho make the screen and the button all one "front panel" unit, and in the back having the teensy soldered to a piece of perfboard, srewed to the frame.
The screen & the USB connector will be connected to the teensy via a JST XH connector. everything clearly labeled on the inside.

maybe i can fit it all on a single piece of perfboard, not reqiering a single wire.



TODO:

- make 3d-printalbe encosure
- make build log / instructions
- add i2c-LCD to Teensy code
- make debug commands via serial
- build the damn thing
