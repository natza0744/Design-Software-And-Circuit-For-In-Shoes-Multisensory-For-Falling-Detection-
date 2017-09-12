#include <CurieTimerOne.h>
#include "CurieIMU.h"

int time = 100000;
float Ax, Ay, Az;
//#define SERIAL_PORT_LOG_ENABLE 1
void UpdateSensor()
{
	CurieIMU.readAccelerometerScaled(Ax, Ay, Az);
// #ifdef SERIAL_PORT_LOG_ENABLE
	Serial.print("a:\t");
  Serial.print(Ax);
  Serial.print("\t");
  Serial.print(Ay);
  Serial.print("\t");
  Serial.print(Az);
  Serial.println();
  Serial.print("Count: ");
  Serial.println(CurieTimerOne.readTickCount());
//#endif
}

void setup() {
 // #ifdef SERIAL_PORT_LOG_ENABLE
  Serial.begin(9600);
 // #endif
  while(!Serial){
      // statement
  }

  CurieIMU.begin();

  CurieIMU.setAccelerometerRange(2);


}

void loop() {
  CurieTimerOne.start(time, &UpdateSensor);
  
  
  
 //#ifdef SERIAL_PORT_LOG_ENABLE
  Serial.print("Count: ");
  Serial.println(CurieTimerOne.rdRstTickCount());
// #endif
  delay(1000);

}
