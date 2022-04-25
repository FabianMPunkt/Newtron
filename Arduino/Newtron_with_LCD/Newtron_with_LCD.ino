//holy shit this is working!!!!
//
//  Newtron Prototype V0.6
//  https://github.com/TheWaschtlPlays/Newtron
//  with LCD!
//
//  This reads incoming data from Serial1, that is seperated by a "".
//  it then determines the maximum value
//  when pin3 is pulled low, it sends said value to the connected computer as if it were keyboard presses.


#include <Keyboard.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

String raw;
String rawlast;
int rawValueInt;
int posValue;
int maxValue = 0;

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
bool lastUSBStatus;
bool USBStatus;


void setup() {

  pinMode (pedalPin, INPUT_PULLUP);
  pinMode (rstPin, INPUT_PULLUP);
  pinMode (lcdPowerPin, OUTPUT);
  pinMode (USBStatusPin, INPUT);

  Keyboard.begin();
  
  raw.reserve(400);                         //I think this is for buffering all the incoming data. i dont really know how this works lol.
  rawlast.reserve(400);

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

  

  lastUSBStatus = USBStatus;

  USBStatus = digitalRead(USBStatusPin);
 

  if (lastUSBStatus == LOW && USBStatus == HIGH){     //detects rising edge of USBStatusPin, and clears the screen.

    Serial.println("USB connected");
    displayON();

    lcd.setCursor(0, 1);
    lcd.print("                ");
    
    lcd.setCursor(0, 1);
    lcd.print(maxValue);
  }



  if (USBStatus) {

    rawlast = raw;                            //has something to do with the buffering.

    raw = Serial1.readStringUntil('');       //reads the incoming data. "" ist the actual seperator.
    rawValueInt = raw.toInt();                //converts String type to Integer type.
    posValue = fabsf(rawValueInt);            //turns negative values into positive values.

    if (posValue > maxValue) {                //largest value gets saves as "maxValue".
      maxValue = posValue;
      displayON();                            //whenever there is a new "maxValue" the LCD will turn on again.
      lcd.setCursor(0, 1);
      lcd.print("      ");
      lcd.setCursor(0, 1);
      lcd.print(maxValue);
    }


    lcd.setCursor(8, 1);
    lcd.print("      ");
    lcd.setCursor(8, 1);
    lcd.print(posValue);


        Serial.print("raw: ");
        Serial.print(raw);
        Serial.print(" | pos: ");
        Serial.print(posValue);
        Serial.print(" | max: ");
        Serial.print(maxValue);
        Serial.println();

  }

    else{
      lcd.setCursor(0,1);
      lcd.print("USB disconnected");
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
  lcd.print("6");
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
