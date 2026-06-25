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



const unsigned long motorOffTime  = 7000;
const unsigned long motorPeriod   = motorOnTime + motorOffTime;
const unsigned long serialInterval = 500; 
unsigned long prevSerialTime = 0;

void setup() {
  Serial.begin(9600);
  delay(2000);

  motorESC.attach(escPin);
  motorESC.writeMicroseconds(1000); 
  delay(3000); 

  Serial.println(F("Система готова"));
}

void loop() {
  unsigned long currentMillis = millis();

  
  unsigned long timeInCycle = currentMillis % motorPeriod;
  if (timeInCycle < motorOnTime) {
    motorESC.writeMicroseconds(1700); 
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
    Serial.println(total, 3);
  }
}