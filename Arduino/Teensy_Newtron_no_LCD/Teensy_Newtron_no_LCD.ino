//
//  Newtron V0.8
//  Teensy 4.1 (without LCD)
//  https://github.com/TheWaschtlPlays/Newtron
//
//
//  This reads incoming data from Serial1, that is seperated by a "".
//  it then determines the maximum value
//  when pin3 is pulled low, it sends said value to the connected computer as if it were keyboard presses.

#include "Keyboard.h"

#include "USBHost_t36.h"
#define USBBAUD 9600
uint32_t baud = USBBAUD;
USBHost myusb;
USBSerial_BigBuffer userial(myusb, 1); // Handles anything up to 512 bytes

USBDriver *drivers[] = {&userial};
#define CNT_DEVICES (sizeof(drivers)/sizeof(drivers[0]))
bool driver_active[CNT_DEVICES] = {false};


String integString;           //innteger part of the value
String decimString;           //decimal point of the value

float integFloat;
float decimFloat;

float finValue;
float maxValue = 0;

int pedalPin = 9;
bool currentPedalState;
bool lastPedalState;


void setup() {

  pinMode (pedalPin, INPUT_PULLUP);
  pinMode (11, OUTPUT);
  pinMode (13, OUTPUT);
  digitalWrite(11, LOW);

  Keyboard.begin();

  myusb.begin();
  Serial1.begin(9600);


}




void loop() {
  
  for (uint8_t i = 0; i < CNT_DEVICES; i++) {     //USB detection. no clue how this works lol.
     if (*drivers[i] != driver_active[i]) {
      
      if (driver_active[i]) {
       driver_active[i] = false;
        Serial.println("USB disconnected");
        digitalWrite(13, LOW);
        rstMaxValue();
        
      }
      
      else {
        driver_active[i] = true;
        userial.begin(baud);
        Serial.println("USB connected");
        digitalWrite(13, HIGH);
        
      }
    }
    
  }
  

  while (userial.available()) {


    integString = userial.readStringUntil(',');       //reads the value before the comma.
    integFloat = integString.toFloat();               //converts String type to Float type
    integFloat = fabsf(integFloat);                   //turns all negative numbers positive
    
    
    decimString = userial.readStringUntil('');       //reads the value after the comma, until the separator ""
    decimFloat = decimString.toFloat();               //converts String type to Float type. the decimal number is now a standalone float variable.
    decimFloat = decimFloat / 10;                     //devides the decimal number by 10.


    finValue = integFloat + decimFloat;               //adds integer number and decimal number.

    if (finValue >= maxValue) {                       //largest value gets saves as "maxValue".
      maxValue = finValue;
    }

        Serial.print("raw: ");
        Serial.print(integString);
        Serial.print(",");
        Serial.print(decimString);
        Serial.print(" | fin: ");
        Serial.print(finValue);
        Serial.print(" | max: ");
        Serial.print(maxValue);
        Serial.println();

    Pedal();

  }
  
}

void Pedal() {

  lastPedalState = currentPedalState;         //this is used to determine the falling edge when the pedal is pressed.
  currentPedalState = digitalRead(pedalPin);

  if (lastPedalState == HIGH && currentPedalState == LOW) {
    
    Serial.println("Pedal has been pressed");
     
    Keyboard.println(maxValue);
    rstMaxValue();
    delay(300);                             //300ms delay so you cant accidentally press the pedal twice.
  }
}

void rstMaxValue() {
  maxValue = 0;
  Serial.println("maxVaule has been reset");
}
