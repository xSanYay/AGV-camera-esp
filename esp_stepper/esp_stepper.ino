#include <Stepper.h>

const int steps_per_rev = 200; //Set to 200 for NIMA 17
#define INA1 14
#define INA2 27
#define INA3 26
#define INA4 25

#define INB1 12
#define INB2 13
#define INB3 15
#define INB4 0

#define INC1 2
#define INC2 14
#define INC3 16
#define INC4 4


Stepper m1(steps_per_rev, INA1, INA2, INA3, INA4);
Stepper m2(steps_per_rev, INB1, INB2, INB3, INB4);
Stepper m3(steps_per_rev, INC1, INC2, INC3, INC4);


void setup()
{
  m1.setSpeed(60);
  Serial.begin(115200);
}

void loop() 
{
  Serial.println("Rotating Clockwise...");
  m1.step(steps_per_rev);
  delay(500);

  Serial.println("Rotating Anti-clockwise...");
  m1.step(-steps_per_rev);
  delay(500);
}
