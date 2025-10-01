# Sensoren proof of concept

minimale hard- en software die aantoont dat minimaal 6 sensoren onafhankelijk van elkaar kunnen uitgelezen worden (geen calibratie, normalisatie of interpolatie). Hierbij moet een zo groot mogelijk bereik van de AD converter benut worden (indien van toepassing)

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
