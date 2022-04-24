//this just spits out A0 readings in the same format as "TesT" Newton-meters



int randNumber;

void setup() {
  Serial.begin(9600);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, LOW);
  
  //randomSeed(analogRead(0));
}

void loop() {

  //randNumber = (random(-3000, 3000));
  //Serial.print(randNumber);

  Serial.print(analogRead(A0) * 10);
    
  Serial.print("  ");  //"" is the real deal

  delay(30);
}
