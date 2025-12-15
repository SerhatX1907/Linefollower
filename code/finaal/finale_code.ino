#include <EEPROM.h>

#define BAUDRATE 9600
#define SerialPort Serial1   // <-- BELANGRIJK: Bluetooth gebruikt Serial1

const int IN1 = 3;
const int IN2 = 9;
const int IN3 = 5;
const int IN4 = 6;
const int SENSOR_PINS[] = {A10, A1, A2, A3, A4, A5, A6, A11};
const float SENSOR_SPACING = 15.0;

struct param_t {
  int black[8];
  int white[8];
  int power;
  float p;
  float d;
} params;

bool running = false;
int raw[8];
long normalised[8];
float position = 0;
float lastPosition = 0;
String inputBuffer = "";

template <class T> int EEPROM_writeAnything(int ee, const T& value) {
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++) EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value) {
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++) *p++ = EEPROM.read(ee++);
    return i;
}

void setup() {
  SerialPort.begin(BAUDRATE);    // <-- Bluetooth via Serial1

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();

  EEPROM_readAnything(0, params);

  SerialPort.println("READY - SERIAL1 VERSION");
}

void loop() {

  // Bluetooth commands inlezen via Serial1
  while (SerialPort.available()) {
    char c = SerialPort.read();
    if (c == '\n' || c == '\r') {
      inputBuffer.trim();
      if (inputBuffer.length() > 0) processCommand(inputBuffer);
      inputBuffer = "";
    } else {
      inputBuffer += c;
    }
  }

  if (running) {

    int index = 0;
    int minVal = 1001;

    for(int i = 0; i < 8; i++) { 
      raw[i] = analogRead(SENSOR_PINS[i]); 

      int range = params.white[i] - params.black[i];
      if (range == 0) range = 1;

      normalised[i] = map(raw[i], params.black[i], params.white[i], 0, 1000);
      normalised[i] = constrain(normalised[i], 0, 1000);

      if (normalised[i] < minVal) {
        minVal = normalised[i];
        index = i;
      }
    }

    if (index == 0) index = 1;
    else if (index == 7) index = 6;

    long sZero = normalised[index];
    long sMinusOne = normalised[index-1];
    long sPlusOne = normalised[index+1];

    float b = ((float)(sPlusOne - sMinusOne)) / 2.0;
    float a = sPlusOne - b - sZero;

    position = 0;
    if (abs(a) > 0.1) {
      position = -b / (2.0 * a);
    }
    position += index - 2.5;   
    position *= SENSOR_SPACING;

    float error = -position;
    float derivative = position - lastPosition;
    float output = (params.p * error) - (params.d * derivative);

    lastPosition = position;

    int left = params.power + output;
    int right = params.power - output;

    left = constrain(left, -255, 255);
    right = constrain(right, -255, 255);

    setMotors(left, right);
  }
}

void setMotors(int left, int right) {
  if (left >= 0) { analogWrite(IN1, left); analogWrite(IN2, 0); }
  else { analogWrite(IN1, 0); analogWrite(IN2, -left); }

  if (right >= 0) { analogWrite(IN3, right); analogWrite(IN4, 0); }
  else { analogWrite(IN3, 0); analogWrite(IN4, -right); }
}

void stopMotors() {
  analogWrite(IN1, 0); analogWrite(IN2, 0);
  analogWrite(IN3, 0); analogWrite(IN4, 0);
}

void processCommand(String cmd) {
  cmd.toLowerCase();

  if (cmd == "start" || cmd == "go") {
    running = true;
    lastPosition = 0;
    SerialPort.println("STARTED");
  }
  else if (cmd == "stop") {
    running = false;
    stopMotors();
    SerialPort.println("STOPPED");
  }
  else if (cmd == "info") {
    SerialPort.print("Power:"); SerialPort.print(params.power);
    SerialPort.print(" P:"); SerialPort.print(params.p);
    SerialPort.print(" D:"); SerialPort.println(params.d);
  }
  else if (cmd == "debug") {

    SerialPort.print("RAW: ");
    for(int i=0; i<8; i++) { 
      raw[i] = analogRead(SENSOR_PINS[i]); 
      SerialPort.print(raw[i]); SerialPort.print(" ");
    }
    SerialPort.println();

    SerialPort.print("NORM: ");
    int index = 0;
    int minVal = 1001;
    
    for (int i = 0; i < 8; i++) {
      int range = params.white[i] - params.black[i];
      if (range == 0) range = 1;
      
      normalised[i] = map(raw[i], params.black[i], params.white[i], 0, 1000);
      normalised[i] = constrain(normalised[i], 0, 1000);
      SerialPort.print(normalised[i]); SerialPort.print(" ");
      
      if (normalised[i] < minVal) {
        minVal = normalised[i];
        index = i;
      }
    }
    SerialPort.println();
  }
  else if (cmd == "cal b") {
    SerialPort.print("BLACK: ");
    for(int i = 0; i < 8; i++) {
      params.black[i] = analogRead(SENSOR_PINS[i]); 
      SerialPort.print(params.black[i]); SerialPort.print(" ");
    }
    SerialPort.println();
    EEPROM_writeAnything(0, params);
  }
  else if (cmd == "cal w") {
    SerialPort.print("WHITE: ");
    for(int i = 0; i < 8; i++) {
      params.white[i] = analogRead(SENSOR_PINS[i]); 
      SerialPort.print(params.white[i]); SerialPort.print(" ");
    }
    SerialPort.println();
    EEPROM_writeAnything(0, params);
  }
  else if (cmd.startsWith("set power ")) {
    params.power = constrain(cmd.substring(10).toInt(), 0, 255);
    EEPROM_writeAnything(0, params);
    SerialPort.print("POWER:"); SerialPort.println(params.power);
  }
  else if (cmd.startsWith("set p ")) {
    params.p = cmd.substring(6).toFloat();
    EEPROM_writeAnything(0, params);
    SerialPort.print("P:"); SerialPort.println(params.p);
  }
  else if (cmd.startsWith("set d ")) {
    params.d = cmd.substring(6).toFloat();
    EEPROM_writeAnything(0, params);
    SerialPort.print("D:"); SerialPort.println(params.d);
  }
  else {
    SerialPort.print("?:"); SerialPort.println(cmd);
  }
}
