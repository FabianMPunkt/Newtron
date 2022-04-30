# Newtron
## A little Arduino-based USB device that reads Serial data from a "TesT" - Newton-meter and transmits it directly to the Computer


Alright then,

this little project is made to read the USBSerial output of one these ["TesT"-devices](https://www.test-gmbh.com/de/produkte/pruefmaschinen/einfache-pruefrahmen/modell-105/).
It will process the readings, turn them all into positive values and save the highes value.

With the press of a button the highest value will the be sent to the computer using the "Keyboard.h" library.


The TesT-device trannsmits alle the values over its USBSerial Port in a single line as seen in [these readings](misc/OG_readings.txt)

Now you may be wondering how we can read this data using an Arduino, if the is a USB-Device.
After searching through virtually every existing Arduino forum there is, i finally found a solution:

These little [USB Host Boards](https://www.hobbytronics.co.uk/usb-host/usb-host-board-v24) sold by HobbyTronics can, among many other cool things, take data from a USBSerial connection, and "convert" it to standart RX / TX signals.
After that, everything else is pretty straigt forward.

ATM i am using an Arduino Leonardo and a generic 16x2 LCD

All this is still WIP.


TODO:

- make 3d-printalbe encosure
- get the decimal point working
