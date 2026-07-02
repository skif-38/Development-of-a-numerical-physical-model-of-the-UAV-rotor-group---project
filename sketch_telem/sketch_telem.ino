#include <Servo.h>
#include <math.h>

Servo motorESC;
const int escPin = 9;

const int pinA0 = A0;
const int pinA1 = A1;
const int pinA2 = A2;
const int pinA3 = A3;

const float vRef = 4.94;
const float ratio0 = 2.032;
const float ratio1 = 3.057;
const float ratio2 = 4.104;
const float ratio3 = 5.104;

int throttle = 1600;

const unsigned long motorOnTime = 10000;
const unsigned long motorOffTime = 5000;
const unsigned long motorPeriod = motorOnTime + motorOffTime;

const unsigned long serialInterval = 500;
unsigned long prevSerialTime = 0;

void setup() {
  Serial.begin(9600);
  delay(2000);

  motorESC.attach(escPin);
  motorESC.writeMicroseconds(1000); 
  delay(3000);                      

  delay(10000);  
  
  Serial.println(F("Старт цикла: 10с работа, 5с пауза"));
}

void loop() {
  unsigned long currentMillis = millis();

  unsigned long timeInCycle = currentMillis % motorPeriod;
  bool motorOn = (timeInCycle < motorOnTime);  

  if (motorOn) {
    motorESC.writeMicroseconds(throttle); 
  } else {
    motorESC.writeMicroseconds(1000);       
  }

  if (currentMillis - prevSerialTime >= serialInterval) {
    prevSerialTime = currentMillis;

    float vB1 = (analogRead(pinA0) / 1024.0) * vRef * ratio0;
    float vB2 = (analogRead(pinA1) / 1024.0) * vRef * ratio1;
    float vB3 = (analogRead(pinA2) / 1024.0) * vRef * ratio2;
    float vB4 = (analogRead(pinA3) / 1024.0) * vRef * ratio3;

    float cell1 = vB1;
    float cell2 = vB2 - vB1;
    float cell3 = vB3 - vB2;
    float cell4 = vB4 - vB3;
    float total = vB4;

    cell1 = fabs(cell1);
    cell2 = fabs(cell2);
    cell3 = fabs(cell3);
    cell4 = fabs(cell4);
    total = fabs(total);

    int gasPercent;
    if (motorOn) {
      gasPercent = (throttle - 1000) / 10;
    } else {
      gasPercent = 0;
    }

    Serial.print(currentMillis);
    Serial.print(F(","));
    Serial.print(cell1, 3);
    Serial.print(F(","));
    Serial.print(cell2, 3);
    Serial.print(F(","));
    Serial.print(cell3, 3);
    Serial.print(F(","));
    Serial.print(cell4, 3);
    Serial.print(F(","));
    Serial.print(total, 3);
    Serial.print(F(","));
    Serial.println(gasPercent);
  }
}