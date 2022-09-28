//this just spits out A0 readings in the same format as "TesT" Newton-meters
//if pin2 is pulled low, it will stop.


void setup() {
  Serial.begin(115200);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, LOW);

  pinMode(2, INPUT_PULLUP);
  
}

void loop() {

if(digitalRead(2) == HIGH) {

    Serial.print(analogRead(A0));
    Serial.print(",");
    Serial.print(analogRead(A0) / 20);
      
    Serial.print("  ");  //"" is the real deal

    delay(30);

  }

}
