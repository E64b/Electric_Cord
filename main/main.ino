#include <Servo.h>
#include <Arduino.h>
#include <inttypes.h>

/*SETUP*/
#define PREPARE_TIME 3000     // Delay after start ms
#define FLYING_TIME 180000    // Time flight ms
#define TIME_STEP 30          // Time step for runing and breaking
#define MAX_RPM 90            // Max rpm 0-100%
#define SENSOR_PIN A1         // RPM sensor pin
#define ENG_PIN 9             // Engine pin
#define IDLE_SPEED 10         // min rpm
#define IDLE_SPEED_TIME 10000 // ms

Servo PID;

uint8_t maxThrottle = MAX_RPM;
uint8_t curThrottle = 0;
uint32_t previousMillis = 0;
bool start = false;
bool breaking = false;
bool delayStart = true;
bool work = true;
float voltage = 0;

void setup() {
  pinMode(SENSOR_PIN, INPUT);
  PID.attach(ENG_PIN, 1000, 2000);
  PID.write(curThrottle);
  Serial.begin(9600);
}

void loop() {
  voltage = (float)(analogRead(SENSOR_PIN) * 5.0) / 1024;

  if (voltage < 0.5) {
    Serial.print("Voltage = ");
    Serial.println(voltage);
    delay(50);
  }

  if (voltage > 0.5 && delayStart) {
    start = true;
    PID.write(IDLE_SPEED);
    delay(IDLE_SPEED_TIME);
  }

  if (work) {
    if (start) {
      for (curThrottle = 0; curThrottle < maxThrottle; curThrottle++) {
        PID.write(curThrottle);
        delay(TIME_STEP);
      }

      uint32_t currentMillis = millis();

      while (millis() - currentMillis <= FLYING_TIME) {
        PID.write(maxThrottle);
        breaking = true;
      }

      if (breaking) {
        for (curThrottle = maxThrottle; curThrottle > 0; curThrottle--) {
          PID.write(curThrottle);
          delay(TIME_STEP);
        }
        PID.write(0);
        breaking = false;
        work = false;
      }
    }
  }
  start = false;
  delayStart = false;
}
