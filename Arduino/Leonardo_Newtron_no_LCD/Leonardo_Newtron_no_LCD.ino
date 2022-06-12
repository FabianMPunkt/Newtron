//
//  Newtron V0.9
//  Arduino Leonardo (without LCD)
//  https://github.com/FabianMPunkt/Newtron
//
//
//  This reads incoming data from Serial1, that is seperated by a "".
//  it then determines the maximum value
//  when pin3 is pulled low, it sends said value to the connected computer as if it were keyboard presses.




#include "Keyboard.h"

String integString;           //innteger part of the value
String decimString;           //decimal point of the value

float integFloat;
float decimFloat;

float finValue;
float maxValue = 0;

const int pedalPin = 3;
const int USBStatusPin = 4;                 //Connected to pin "SS" on USB Host
bool currentPedalState;
bool lastPedalState;
bool lastUSBStatus;
bool USBStatus;

int rstPin = 2;

void setup() {

  pinMode (pedalPin, INPUT_PULLUP);
  pinMode (rstPin, INPUT_PULLUP);
  pinMode (USBStatusPin, INPUT);

  Serial.begin(9600);
  Serial1.begin(9600);

  Keyboard.begin();
  
}

void loop() {


  lastUSBStatus = USBStatus;

  USBStatus = digitalRead(USBStatusPin);
 

  if (lastUSBStatus == LOW && USBStatus == HIGH){     //detects rising edge of USBStatusPin, and clears the screen.

    Serial.println("USB connected");
  }

  if (lastUSBStatus == HIGH && USBStatus == LOW){     //detects falling edge of USBStatusPin, and clears the screen.

    Serial.println("USB disconnected");
    rstMaxValue();
  }


  if (USBStatus) {


    integString = Serial1.readStringUntil(',');       //reads the value before the comma.
    integFloat = integString.toFloat();               //converts String type to Float type
    integFloat = fabsf(integFloat);                   //turns all negative numbers positive
    
    
    decimString = Serial1.readStringUntil('');       //reads the value after the comma, until the separator ""
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
  if (digitalRead(rstPin) == LOW){            //reset "maxValue" 
    rstMaxValue();
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
