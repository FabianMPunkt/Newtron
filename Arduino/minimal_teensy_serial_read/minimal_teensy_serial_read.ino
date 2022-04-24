//  This is an extremely simplified version of the Teensy 3.6 USB Host Serial example:
//  https://github.com/PaulStoffregen/USBHost_t36/blob/master/examples/Serial/Serial.ino
//
//  This sketch will read USBSerial data from a USB device connected to the teensy's USB Host pins.
//  It will also detect whether USB is connected or not.
//
//

#include "USBHost_t36.h"
#define USBBAUD 9600
uint32_t baud = USBBAUD;
USBHost myusb;

USBSerial_BigBuffer userial(myusb, 1); // Handles anything up to 512 bytes

USBDriver *drivers[] = {&userial};
#define CNT_DEVICES (sizeof(drivers)/sizeof(drivers[0]))
bool driver_active[CNT_DEVICES] = {false};


void setup() {

  myusb.begin();
  Serial1.begin(9600);

}


void loop() {
  
  for (uint8_t i = 0; i < CNT_DEVICES; i++) {     //USB detection. no clue how this works lol.
     if (*drivers[i] != driver_active[i]) {
      
      if (driver_active[i]) {
       driver_active[i] = false;
        Serial.println();
        Serial.println("disconnected");
        
      } else {  
        driver_active[i] = true;
        userial.begin(baud);
        Serial.println("connected");
        
      }
    }
    
  }
  

  while (userial.available()) {
    Serial.write(userial.read());
  }
  
}
