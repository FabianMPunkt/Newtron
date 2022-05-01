//this just spits out A0 readings in the same format as "TesT" Newton-meters


void setup() {
  Serial.begin(9600);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, LOW);
  
}

void loop() {


  Serial.print(analogRead(A0));
  Serial.print(",");
  Serial.print(analogRead(A0) / 20);
    
  Serial.print("  ");  //"" is the real deal

  delay(30);
}
