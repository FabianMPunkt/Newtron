String Version = "Newtron V1.2";

//  Teensy 4.1 + I2C LCD
//  https://github.com/FabianMPunkt/Newtron
//
//
//  USB Type: "Serial + Keyboard + Mouse + Joystick"
//  CPU Speed: 396MHz
//
//
//  This reads incoming data from Serial1, that is seperated by a "".
//  it then determines the maximum value
//  when pin3 is pulled low, it sends said value to the connected computer as if it were keyboard presses.


// Update 03.11.2023: 
// inside USBHost_t36/serial.cpp change

// // Lets try to map CDCACM devices only at device level
//  	if ((dev->bDeviceClass == 2) && (dev->bDeviceSubClass == 0)) {
//  		if (type != 0) return false;

// to

// 	// Lets try to map CDCACM devices only at device level
//	if ((dev->bDeviceClass == 2) && (dev->bDeviceSubClass == 0) && ((type == 0) )) {
//
// this way it will work with newer USB-C TesT-devices.
// https://forum.pjrc.com/threads/73829-Need-help-with-USBSerial-device-not-getting-recognized-by-teensy-4-1?p=333498#post333498

#include "LCD_I2C.h"
LCD_I2C lcd(0x27, 16, 2);  //Defines LCD Type

#include "Keyboard.h"

#include "USBHost_t36.h"
#define USBBAUD 115200  //BAUDRADE for the Serial of the USB-Host
uint32_t baud = USBBAUD;
USBHost myusb;
USBSerial_BigBuffer userial(myusb);

USBDriver *drivers[] = { &userial };
#define CNT_DEVICES (sizeof(drivers) / sizeof(drivers[0]))
bool driver_active[CNT_DEVICES] = { false };



String integString;  //innteger part of the value
String decimString;  //decimal part of the value

float integFloat;
float decimFloat;

float finValue;
float maxValue = 0;

unsigned long millisUntilDisplayTimeout;
unsigned long displayTimeoutValue = 90000;  //timeout Value for the LCD, when inactive to prevent LCD burn-in. (using millis)

unsigned long millisUntilDataTimeout;
unsigned long dataTimeoutValue = 2000;  //timeout Value for datastream detection.

bool dataState = false;  //true if data is detected, false if not.
bool USBState = false;   //true if usb is connected, fasle if not.

const int rstPin = 11;
const int pedalPin = 8;
bool currentPedalState;
bool lastPedalState;

int animDelay = 20;

void setup() {

  bootAnimStart();  //Start sequence of the boot animation


  pinMode(pedalPin, INPUT_PULLUP);
  pinMode(rstPin, INPUT_PULLUP);

  pinMode(7, OUTPUT);  //Pin 7 and 10 are used as GND
  pinMode(10, OUTPUT);
  digitalWrite(7, LOW);
  digitalWrite(10, LOW);


  Keyboard.begin();

  Serial.begin(9600);

  Serial.println(Version);
  Serial.println("https://github.com/FabianMPunkt/Newtron");

  myusb.begin();


  bootAnimEnd();  //End sequence of the boot animation

  lcd.setCursor(0, 0);
  lcd.print("USB disconnected");  //"USB not found" message is hardcoded so it display correctly, if USB is disconnected when booting. Yes, i know it is stupid
}



void loop() {

  for (uint8_t i = 0; i < CNT_DEVICES; i++) {  //USB detection ripped from: https://github.com/PaulStoffregen/USBHost_t36/blob/master/examples/Serial/Serial.ino
    if (*drivers[i] != driver_active[i]) {

      if (driver_active[i]) {
        driver_active[i] = false;
        USBState = false;
        Serial.println("USB disconnected");
        rstMaxValue();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("USB disconnected");

      }

      else {
        driver_active[i] = true;
        userial.begin(baud);
        USBState = true;
        Serial.println("USB connected");

        lcd.clear();
        rstMaxValue();

        dataState = 0;
      }
    }
  }


  while (userial.available()) {

    millisUntilDataTimeout = millis() + dataTimeoutValue;  //for datastream detection.

    if (dataState == false) {  //detects when data is coming, and runs this only once.
      lcd.setCursor(0, 0);
      lcd.print("Spitzenwert:    ");
      lcd.setCursor(0, 1);

      dataState = true;
      Serial.println("Datastream detected!");

      rstMaxValue();
    }

    integString = userial.readStringUntil(',');  //reads the value before the comma.
    integFloat = integString.toFloat();          //converts String type to Float type
    integFloat = fabsf(integFloat);              //turns all negative numbers positive


    decimString = userial.readStringUntil('');  //reads the value after the comma, until the separator ""
    decimFloat = decimString.toFloat();          //converts String type to Float type. the decimal number is now a standalone float variable.
    decimFloat = decimFloat / 10;                //devides the decimal number by 10.


    finValue = integFloat + decimFloat;  //adds integer number and decimal number.

    if (finValue > maxValue) {  //largest value gets saves as "maxValue".
      maxValue = finValue;
      displayON();  //whenever there is a new "maxValue" the LCD will turn on again.
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
    Serial.print((millisUntilDisplayTimeout - millis()) / 1000);
    Serial.print(" | userial: ");
    Serial.print(userial.available());
    Serial.println();


    rstButton();

    Pedal();

    displayTimeout();
  }

  dataStreamTimeout();

  rstButton();

  displayTimeout();
}


void rstButton() {
  if (digitalRead(rstPin) == LOW) {  //reset "maxValue"
    rstMaxValue();
  }
}


void Pedal() {

  lastPedalState = currentPedalState;  //this is used to determine the falling edge when the pedal is pressed.
  currentPedalState = digitalRead(pedalPin);

  if (lastPedalState == HIGH && currentPedalState == LOW) {

    Serial.println("Pedal has been pressed");

    Keyboard.println(maxValue);
    rstMaxValue();
    delay(500);  //500ms delay so you cant accidentally press the pedal twice.
  }
}


void rstMaxValue() {
  displayON();
  maxValue = 0;
  lcd.setCursor(0, 1);
  lcd.print("                ");
  Serial.println("maxVaule has been reset");
}


void displayON() {
  millisUntilDisplayTimeout = millis() + displayTimeoutValue;

  lcd.backlight();
  lcd.display();
}


void displayTimeout() {

  if (millis() > millisUntilDisplayTimeout) {  //this turns off the LCD when the timeout is reached.
    lcd.noBacklight();
    lcd.noDisplay();
  }
}


void dataStreamTimeout() {  //if no data is coming for more than 2000ms, it will show this error.
  if (millis() > millisUntilDataTimeout) {

    if (USBState == true) {

      lcd.setCursor(0, 0);
      lcd.print("No data found!");
      lcd.setCursor(0, 1);
      lcd.print("DSI Transmission");
      dataState = false;
      Serial.println("No data found!");
    }
  }
}


void bootAnimStart() {  //fancy boot animation cus why not

  lcd.begin();
  displayON();

  lcd.setCursor(0, 0);
  lcd.print(Version.substring(0, 1));
  delay(animDelay);
  lcd.setCursor(1, 0);
  lcd.print(Version.substring(1, 2));
  delay(animDelay);
  lcd.setCursor(2, 0);
  lcd.print(Version.substring(2, 3));
  delay(animDelay);
  lcd.setCursor(3, 0);
  lcd.print(Version.substring(3, 4));
  delay(animDelay);
  lcd.setCursor(4, 0);
  lcd.print(Version.substring(4, 5));
  delay(animDelay);
  lcd.setCursor(5, 0);
  lcd.print(Version.substring(5, 6));
  delay(animDelay);
  lcd.setCursor(6, 0);
  lcd.print(Version.substring(6, 7));
  delay(animDelay);
  lcd.setCursor(7, 0);
  lcd.print(Version.substring(7, 8));
  delay(animDelay);
  lcd.setCursor(8, 0);
  lcd.print(Version.substring(8, 9));
  delay(animDelay);
  lcd.setCursor(9, 0);
  lcd.print(Version.substring(9, 10));
  delay(animDelay);
  lcd.setCursor(10, 0);
  lcd.print(Version.substring(10, 11));
  delay(animDelay);
  lcd.setCursor(11, 0);
  lcd.print(Version.substring(11, 12));
  delay(animDelay);
  lcd.setCursor(12, 0);
  lcd.print(Version.substring(12, 13));
  delay(animDelay);
}

void bootAnimEnd() {

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
  delay(animDelay);
}