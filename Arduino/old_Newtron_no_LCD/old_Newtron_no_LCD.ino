//kek
//this only reads incoming serial data, determines the maximum Value, and passes it along to a computer with the press of a button
// WORKING!!!!!
#include "Keyboard.h"

String rawValue;
float rawValueFloat;
float posValue;
float maxValue = 0;

int pedalPin = 3;
bool currentPedalState;
bool lastPedalState;

int rstPin = 2;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode (pedalPin, INPUT_PULLUP);
  pinMode (rstPin, INPUT_PULLUP);

  Keyboard.begin();

}

void loop() {

  if (Serial1.available() > 0) {
    
    rawValue = Serial1.readStringUntil('');  //reads the incoming data. "" ist the actual seperator.

    rawValueFloat = rawValue.toFloat();       //converts String type to float type.

    posValue = fabsf(rawValueFloat);          //turns negative values into positive values.

    if (posValue >= maxValue) {               //largest value gets saves as "maxValue".
      maxValue = posValue;
    }

    Serial.print("raw: ");
    Serial.print(rawValue);
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

void Pedal(){
  
  lastPedalState = currentPedalState;         //this is used to determine the negative edge when the pedal is presed.
  currentPedalState = digitalRead(pedalPin);

  if (lastPedalState == HIGH && currentPedalState == LOW) {
    if (maxValue > 20) {                      //if the value is greater than 20, it gets printed to the computer.
      Keyboard.println(maxValue);
      maxValue = 0;
      delay(300);                             //300ms delay so you cant accidentally press the pedal twice.
    }
    else {                                    //or else, it just sends an "enter" command.
      Keyboard.println();
      delay(50);
    }
  }
}


void rstMaxValue();{
  maxValue = 0;
}
