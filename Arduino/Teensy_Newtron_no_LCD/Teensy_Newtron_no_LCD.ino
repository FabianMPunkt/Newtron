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


String raw;
String rawlast;
int rawValueInt;
int posValue;
int maxValue = 0;

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

  raw.reserve(200);
  rawlast.reserve(200);

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

    rawlast = raw;                            //has something to do with the buffering.
    
    raw = userial.readStringUntil('');       //reads the incoming data. "" ist the actual seperator.
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
