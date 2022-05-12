# Newtron
## A little Arduino-based USB device that reads Serial data from a "TesT" - Newton-meter and transmits it directly to the Computer


## Introduction:

This little project is made to read the USBSerial output of one these ["TesT"-devices](https://www.test-gmbh.com/de/produkte/pruefmaschinen/einfache-pruefrahmen/modell-105/).
It will process the readings, turn them all into positive values and save the highest value.

With the press of a button the highest value will the be sent to the computer using the "Keyboard.h" library.


The TesT-device trannsmits all the values over its USBSerial Port in a single line as seen in [these readings](misc/OG_readings.txt).
GitHub's formatting doen't disply this correctly though. All the values are in-fact separated by a ""

Now you may be wondering how we can read this data using an Arduino, if the is a USB-Device.
After searching through virtually every existing Arduino forum there is, i finally found a solution:


### Arduino:

At first i was using one of these little [USB Host Boards](https://www.hobbytronics.co.uk/usb-host/usb-host-board-v24), sold by HobbyTronics and an Arduino Leonardo.
These Host boards can, among many other cool things, take data from a USBSerial connection, and "convert" it to standard RX / TX signals.
After that, reading the Serial Port using the Arduino would be very simple.
All that code is still present.
(files labeled with "Leonardo")


### Teensy:

Then i figured out that all this would be much simpler using a Teensy 4.1, as it has a USB Host built in.
It's essentially the same, only with some extra USB code that i ripped from [here](https://github.com/PaulStoffregen/USBHost_t36/blob/master/examples/Serial/Serial.ino).
(files labeled with "Teensy")


In the final project i will also be using a 16x2LCD with an i2c add-on.
I will also be making some building Instructions.

All this is still WIP.


TODO:

- make 3d-printalbe encosure
- add i2c-LCD to Teensy code
- make debug commands via serial
- build the damn thing
