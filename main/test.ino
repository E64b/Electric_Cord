#include <Arduino.h>
#include <inttypes.h>

#define SENSOR_PIN A1

float voltage = 0;

void setup() {
  pinMode(SENSOR_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  voltage = (float)(analogRead(pin) * 5.0) / 1024;
  Serial.print("Voltage = ");
  Serial.println(voltage);
  delay(50);
}
