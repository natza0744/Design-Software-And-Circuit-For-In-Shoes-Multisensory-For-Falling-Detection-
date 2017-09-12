#include <Arduino_FreeRTOS.h>
#include "CurieIMU.h"
#include <CurieIMU.h>
#include <CurieBLE.h>
#include "CurieTimerOne.h"


void TaskSendData( void *pvParameters );
void TaskReadSensor( void *pvParameters );
typedef struct accelerometer
{
	float AxStore[9];  
	float AyStore[9];
	float AzStore[9];
	float AxSend[9];
	float AySend[9];	
	float AzSend[9];

}Accelerometer;

Accelerometer Acc;
Accelerometer *a
a = &Acc;

int i = 0;


void setup() {

	// This task for send data
	xTaskCreate(
	  	TaskSendData,
	  	(const portCHAR *)"SendData",
	  	128,
	  	NULL,
	  	2,
	  	NULL
	  	);
	xTaskCreate(
	  	TaskReadSensor,
	  	(const portCHAR *)"ReadSensor",
	  	128,
	  	NULL,
	  	2,
	  	NULL
	  	);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void TaskSendData(void *pvParameters)
{
	(void) pvParameters;
	for(;;)
	{
		//send data
	}
}

void TaskReadSensor(void *pvParameters)
{
	(void) pvParameters;
	Serial.begin(9600);

	CurieIMU.begin();

	CurieIMU.setAccelerometerRange(2);

	for(;;)
	{
		CurieIMU.ReadAccelerometerScaled(Acc.AxStore[i], Acc.AyStore[i], Acc.AzStore[i])
		Serial.print("X:") Serial.print(Acc.AxStore[i]); Serial.print("\t");
		Serial.print(Acc.AyStore[i]); Serial.print("\t");
		Serial.print(Acc.AzStore[i]);
	}
}
