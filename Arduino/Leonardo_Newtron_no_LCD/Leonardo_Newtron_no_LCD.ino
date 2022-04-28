//
//  Newtron V0.8
//  Arduino Leonardo (without LCD)
//  https://github.com/TheWaschtlPlays/Newtron
//
//
//  This reads incoming data from Serial1, that is seperated by a "".
//  it then determines the maximum value
//  when pin3 is pulled low, it sends said value to the connected computer as if it were keyboard presses.




#include "Keyboard.h"

String raw;
String rawlast;
float rawValueFloat;
float posValue;
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

  raw.reserve(400);                         //I think this is for buffering all the incoming data. i dont really know how this works lol.
  rawlast.reserve(400);
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

    rawlast = raw;                            //has something to do with the buffering.
    
    raw = Serial1.readStringUntil('');       //reads the incoming data. "" ist the actual seperator.
    rawValueFloat = raw.toFloat();                //converts String type to Integer type.
    posValue = fabsf(rawValueFloat);            //turns negative values into positive values.

    if (posValue >= maxValue) {               //largest value gets saves as "maxValue".
      maxValue = posValue;
    }

        Serial.print("raw: ");
        Serial.print(raw);
        Serial.print(" | pos: ");
        Serial.print(posValue);
        Serial.print(" | max: ");
        Serial.print(maxValue);
        Serial.println();
  }

  Pedal();

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
