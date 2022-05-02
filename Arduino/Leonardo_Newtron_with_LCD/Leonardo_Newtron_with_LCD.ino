//holy shit this is working!!!!
//
//  Newtron V0.9
//  Arduino Leonardo + LCD
//  https://github.com/TheWaschtlPlays/Newtron
//
//
//  This reads incoming data from Serial1, that is seperated by a "".
//  it then determines the maximum value
//  when pin3 is pulled low, it sends said value to the connected computer as if it were keyboard presses.


#include <Keyboard.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

String integString;           //innteger part of the value
String decimString;           //decimal point of the value

float integFloat;
float decimFloat;

float finValue;
float maxValue = 0;

unsigned long currentMillis;
unsigned long millisUntilTimeout;
unsigned long timeoutValue = 30000;         //timeout Value for the LCD, when inactive to prevent LCD burn-in. (using millis)

const int lcdPowerPin = 6;
const int rstPin = 2;
const int pedalPin = 3;
const int USBStatusPin = 4;                 //Connected to pin "SS" on USB Host
bool currentPedalState;
bool lastPedalState;
bool lcdStatus;
bool lastUSBStatus = 0;
bool USBStatus;

bool USBFlag1;
bool USBFlag2;

void setup() {

  pinMode (pedalPin, INPUT_PULLUP);
  pinMode (rstPin, INPUT_PULLUP);
  pinMode (lcdPowerPin, OUTPUT);
  pinMode (USBStatusPin, INPUT);

  Keyboard.begin();

  bootAnim();                               //run this before the Serial begin stuff or things will get fucked.

  Serial.begin(9600);                       //Serial goes to the Computer
  Serial1.begin(9600);                      //Serial1 goes to the Host Controller.

  lcd.setCursor(0, 0);
  lcd.print("Spitzenwert:");
  displayON();                              //the "displayON" function will turn on the LCD and reset the timeout.

  maxValue = 0;
}

void loop() {

  currentMillis = millis();

  if (((millisUntilTimeout - currentMillis) / 1000) <= 9 ) {              //when the timeout is only 9 seconds away, it will show a countdown on the top right.
    lcd.setCursor(15, 0);
    lcd.print((millisUntilTimeout - currentMillis) / 1000);
  }


  if ((currentMillis > millisUntilTimeout) && lcdStatus == true) {        //this turns off the LCD when the timeout is reached.
    displayOFF();
  }


  USBStatus = digitalRead(USBStatusPin);


  if (USBStatus == HIGH) {                //detects rising edge of USBStatusPin, and clears the screen.
    if (USBFlag1 == LOW) {

      USBFlag1 = 1;

      Serial.println("USB connected");
      displayON();

      lcd.setCursor(0, 1);
      lcd.print("                ");

      lcd.setCursor(0, 1);
      lcd.print(maxValue);
    }
  }

  else {
    USBFlag1 = 0;
  }


  if (USBStatus == LOW) {                  //detects falling edge of USBStatusPin, and clears the screen.
    if (USBFlag2 == LOW) {

      USBFlag2 = 1;

      Serial.println("USB disconnected");
      rstMaxValue();

      lcd.setCursor(0, 1);
      lcd.print("USB disconnected");

    }
  }

  else {
    USBFlag2 = 0;
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
      displayON();                            //whenever there is a new "maxValue" the LCD will turn on again.
      lcd.setCursor(0, 1);
      lcd.print("      ");
      lcd.setCursor(0, 1);
      lcd.print(maxValue);
    }


    lcd.setCursor(8, 1);
    lcd.print("      ");
    lcd.setCursor(8, 1);
    lcd.print(finValue);


    Serial.print("raw: ");
    Serial.print(integString);
    Serial.print(",");
    Serial.print(decimString);
    Serial.print(" | fin: ");
    Serial.print(finValue);
    Serial.print(" | max: ");
    Serial.print(maxValue);
    Serial.println();
  }


  Pedal();


  if (digitalRead(rstPin) == LOW) {           //reset "maxValue"
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
  displayON();
  maxValue = 0;
  lcd.setCursor(0, 1);
  lcd.print("      ");
  Serial.println("maxVaule has been reset");
}


void displayON() {
  millisUntilTimeout = currentMillis + timeoutValue;
  digitalWrite(lcdPowerPin, HIGH);
  lcd.display();
  lcdStatus = true;

  lcd.setCursor(15, 0);
  lcd.print(" ");
}


void displayOFF() {
  lcd.noDisplay();
  digitalWrite(lcdPowerPin, LOW);
  lcdStatus = false;
}

void bootAnim() {                 //fancy boot animation cus why not
  int animDelay = 30;


  digitalWrite(lcdPowerPin, HIGH);
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("N");
  delay(animDelay);
  lcd.setCursor(1, 0);
  lcd.print("e");
  delay(animDelay);
  lcd.setCursor(2, 0);
  lcd.print("w");
  delay(animDelay);
  lcd.setCursor(3, 0);
  lcd.print("t");
  delay(animDelay);
  lcd.setCursor(4, 0);
  lcd.print("r");
  delay(animDelay);
  lcd.setCursor(5, 0);
  lcd.print("o");
  delay(animDelay);
  lcd.setCursor(6, 0);
  lcd.print("n");
  delay(animDelay);
  lcd.setCursor(7, 0);
  lcd.print(" ");
  delay(animDelay);
  lcd.setCursor(8, 0);
  lcd.print("V");
  delay(animDelay);
  lcd.setCursor(9, 0);
  lcd.print("0");
  delay(animDelay);
  lcd.setCursor(10, 0);
  lcd.print(".");
  delay(animDelay);
  lcd.setCursor(11, 0);
  lcd.print("9");
  delay(1000);

  lcd.setCursor(0, 0);
  lcd.print("  ");
  delay(animDelay);
  lcd.print("  ");
  delay(animDelay);
  lcd.print("  ");
  delay(animDelay);
  lcd.print("  ");
  delay(animDelay);
  lcd.print("  ");
  delay(animDelay);
  lcd.print("  ");

  delay(300);
}
