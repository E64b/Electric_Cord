#include <Servo.h>

/*SETUP*/
#define PREPARE_TIME 3000   //Delay after start ms
#define FLYING_TIME 180000  //Time flight ms
#define TIME_STEP 30        //Time step for runing and breaking
#define MAX_RPM 90          //Max rpm 0-100%

Servo PID;

uint8_t maxThrottle = MAX_RPM;
uint8_t curThrottle = 0;
uint32_t previousMillis = 0;
bool start;
bool breaking = false;
bool delayStart = true;

void setup() {
  PID.attach(9, 1000, 2000);
  PID.write(curThrottle);
}

void loop() {
  if (delayStart) {
    PID.write(0);
    delay(PREPARE_TIME);
    start = true;
  }

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
    }
  }
  start = false;
  delayStart = false;
}
