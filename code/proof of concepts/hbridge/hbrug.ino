const int IN1 = 3;  // motor A forward
const int IN2 = 4;   // motor A reverse
const int IN3 = 7;   // motor B forward
const int IN4 = 8;   // motor B reverse

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  // Voorwaarts versnellen/vertragen
  for (int i = 0; i <= 255; i++) {
    analogWrite(IN1, i); analogWrite(IN2, 0);
    analogWrite(IN3, i); analogWrite(IN4, 0);
    delay(10);
  }
  for (int i = 255; i >= 0; i--) {
    analogWrite(IN1, i); analogWrite(IN2, 0);
    analogWrite(IN3, i); analogWrite(IN4, 0);
    delay(10);
  }

  // STOP
  analogWrite(IN1,0); analogWrite(IN2,0);
  analogWrite(IN3,0); analogWrite(IN4,0);
  delay(1000);

  // Achterwaarts versnellen/vertragen
  for (int i = 0; i <= 255; i++) {
    analogWrite(IN1, 0); analogWrite(IN2, i);
    analogWrite(IN3, 0); analogWrite(IN4, i);
    delay(10);
  }
  for (int i = 255; i >= 0; i--) {
    analogWrite(IN1, 0); analogWrite(IN2, i);
    analogWrite(IN3, 0); analogWrite(IN4, i);
    delay(10);
  }

  // STOP
  analogWrite(IN1,0); analogWrite(IN2,0);
  analogWrite(IN3,0); analogWrite(IN4,0);
  delay(1000);
}