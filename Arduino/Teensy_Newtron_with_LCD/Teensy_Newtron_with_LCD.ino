//
//  Newtron V0.9
//  Teensy 4.1 + I2C LCD
//  https://github.com/FabianMPunkt/Newtron
//
//
//  USB Type: "Keyboard"
//  CPU Speed: 396MHz
//
//
//  This reads incoming data from Serial1, that is seperated by a "".
//  it then determines the maximum value
//  when pin3 is pulled low, it sends said value to the connected computer as if it were keyboard presses.


#include "LCD_I2C.h"
LCD_I2C lcd(0x27, 16, 2);       //Defines LCD Type

#include "Keyboard.h"

#include "USBHost_t36.h"
#define USBBAUD 115200          //BAUDRADE for the Serial of the USB-Host
uint32_t baud = USBBAUD;
USBHost myusb;
USBSerial_BigBuffer userial(myusb, 1);

USBDriver *drivers[] = {&userial};
#define CNT_DEVICES (sizeof(drivers)/sizeof(drivers[0]))
bool driver_active[CNT_DEVICES] = {false};



String integString;           //innteger part of the value
String decimString;           //decimal part of the value

float integFloat;
float decimFloat;

float finValue;
float maxValue = 0;

unsigned long millisUntilTimeout;
unsigned long timeoutValue = 90000;         //timeout Value for the LCD, when inactive to prevent LCD burn-in. (using millis)

const int rstPin = 11;
const int pedalPin = 8;
bool currentPedalState;
bool lastPedalState;



void setup() {

  pinMode (pedalPin, INPUT_PULLUP);
  pinMode (rstPin, INPUT_PULLUP);

  pinMode (7, OUTPUT);               //Pin 7 and 10 are used as GND
  pinMode (10, OUTPUT);
  digitalWrite(7, LOW);
  digitalWrite(10, LOW);

  Keyboard.begin();

  bootAnim();

  myusb.begin();
  Serial.begin(9600);

  if (userial.available()== false){
        lcd.setCursor(0, 0);
        lcd.print("USB not found!");    
  }

  maxValue = 0;

}



void loop() {

  for (uint8_t i = 0; i < CNT_DEVICES; i++) {     //USB detection ripped from: https://github.com/PaulStoffregen/USBHost_t36/blob/master/examples/Serial/Serial.ino
    if (*drivers[i] != driver_active[i]) {

      if (driver_active[i]) {
        driver_active[i] = false;
        Serial.println("USB disconnected");
        rstMaxValue();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("USB not found!");

      }

      else {
        driver_active[i] = true;
        userial.begin(baud);
        Serial.println("USB connected");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Spitzenwert:");

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

    if (finValue > maxValue) {                        //largest value gets saves as "maxValue".
      maxValue = finValue;
      displayON();                                    //whenever there is a new "maxValue" the LCD will turn on again. TESTING NEEDED! I think this is really slow
      lcd.setCursor(0, 1);
      lcd.print(maxValue);
    }


    //    lcd.setCursor(8, 1);
    //    lcd.print("      ");
    //    lcd.setCursor(8, 1);
    //    lcd.print(finValue);
    

    Serial.print("fin: ");
    Serial.print(finValue);
    Serial.print(" | max: ");
    Serial.print(maxValue);
    Serial.print(" | time: ");
    Serial.print((millisUntilTimeout - millis() ) / 1000);
    Serial.println();


    if (digitalRead(rstPin) == LOW) {           //reset "maxValue"
      rstMaxValue();
    }

    Pedal();

    displayTimeout();

  }

  displayTimeout();

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
  millisUntilTimeout = millis() + timeoutValue;

  lcd.backlight();
  lcd.display();
}


void displayTimeout() {

  if (millis() > millisUntilTimeout) {        //this turns off the LCD when the timeout is reached.
    lcd.noBacklight();
    lcd.noDisplay();
  }
  
}


void bootAnim() {                 //fancy boot animation cus why not
  int animDelay = 30;


  lcd.begin();
  displayON();

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
