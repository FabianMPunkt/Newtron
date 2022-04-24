//kek
//this only reads incoming serial data, determines the maximum Value, and passes it along to a computer with the press of a button
// WORKING!!!!!
#include "Keyboard.h"

String raw;
String rawlast;
int rawValueInt;
int posValue;
int maxValue = 0;

int pedalPin = 3;
bool currentPedalState;
bool lastPedalState;

int rstPin = 2;

void setup() {

  pinMode (pedalPin, INPUT_PULLUP);
  pinMode (rstPin, INPUT_PULLUP);

  Serial.begin(9600);
  Serial1.begin(9600);

  Keyboard.begin();

  raw.reserve(400);                         //I think this is for buffering all the incoming data. i dont really know how this works lol.
  rawlast.reserve(400);
}

void loop() {

  if (Serial1.available() > 0) {

    rawlast = raw;                            //has something to do with the buffering.
    
    raw = Serial1.readStringUntil('');       //reads the incoming data. "" ist the actual seperator.
    rawValueInt = raw.toInt();                //converts String type to Integer type.
    posValue = fabsf(rawValueInt);            //turns negative values into positive values.

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

    Keyboard.println(maxValue);
    rstMaxValue();
    delay(300);                             //300ms delay so you cant accidentally press the pedal twice.
  }
}


void rstMaxValue() {
  maxValue = 0;
}
