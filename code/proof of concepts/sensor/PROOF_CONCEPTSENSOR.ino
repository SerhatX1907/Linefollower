

int sensorPins[6] = {A5, A4, A3, A2, A1, A0};

void setup() {
  Serial.begin(9600);

  
}

void loop() {
  for (int i = 0; i < 6; i++) {
    int waarde = analogRead(sensorPins[i]);
    Serial.print(waarde);
    Serial.print(" ");
  }
  Serial.println();
  delay(500);
}