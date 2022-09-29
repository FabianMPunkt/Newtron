//this just spits out A0 readings in the same format as "TesT" Newton-meters
//if pin2 is pulled low, it will stop.

bool enabled = false;

void setup() {

  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, LOW);

  pinMode(2, INPUT_PULLUP);
}

void loop() {

  if (digitalRead(2) == HIGH) {

    if (enabled == false) {

      Serial.begin(115200);

      Serial.println();
      Serial.println("DSI Transmission enabled");
      enabled = true;

      delay(100);
    }

    Serial.print(analogRead(A0));
    Serial.print(",");
    Serial.print(analogRead(A0) / 20);

    Serial.print("  ");  //"" is the real deal

    delay(10);

  }

  else {

    if (enabled == true) {

      Serial.println();
      Serial.println("DSI Transmission disabled");

      Serial.end();
      enabled = false;

      delay(100);
    }
  }
}