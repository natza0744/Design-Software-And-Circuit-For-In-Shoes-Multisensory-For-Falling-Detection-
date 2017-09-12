#include <BMI160.h>
#include "CurieIMU.h"
#include <CurieIMU.h>
#include <CurieBLE.h>
#include "CurieTimerOne.h"

// Comment the following statement to disable logging on serial port.
//#define SERIAL_PORT_LOG_ENABLE 1

//BLEPeripheral blePeripheral;

BLEService footService("d7344650-9cf3-4df5-9a21-0fcbd7a35b3f");

BLEUnsignedCharCharacteristic footCharAx("d7344651-9cf3-4df5-9a21-0fcbd7a35b3f",BLERead | BLENotify); //uuid for x-axis
BLEUnsignedCharCharacteristic footCharAy("d7344652-9cf3-4df5-9a21-0fcbd7a35b3f",BLERead | BLENotify); //uuid for y-axis
BLEUnsignedCharCharacteristic footCharAz("d7344653-9cf3-4df5-9a21-0fcbd7a35b3f",BLERead | BLENotify); //uuid for z-axis


// number of element in array is 9 because i need to sampling 10 value in 1 second
typedef struct accelerometer
{
	float AxStore;  
	float AyStore;
	float AzStore;
	float AxSend[];
	float AySend[];	
	float AzSend[];

}Accelerometer;

typedef struct Gyroscope
{
	float Gx[9];
	float Gy[9];
	float Gz[9];
	
}Gyroscope;

//float axRaw, ayRaw, azRaw;
int i = 0; // Count Element In Array 
int time = 100000;
Accelerometer Acc;

//Accelerometer *aSend = Acc;
//Accelerometer *aStore = Acc;


void setup()
{
		Serial.begin(9600);
		while(!Serial){
		    // Wainting for the serial port open
		}

	BLE.begin();

	CurieIMU.begin();

	CurieIMU.setAccelerometerRange(2);

	BLE.setLocalName("FOOT");

	//BLE.setAdvertiseService("footService");
	BLE.setAdvertisedServiceUuid(footService.uuid());

	//Add the characteristic to the service
	footService.addCharacteristic(footCharAx);
	footService.addCharacteristic(footCharAy);
	footService.addCharacteristic(footCharAz);

	BLE.addService(footService);

	//Start advertise
	BLE.advertise();
	

}

void loop()
{

	//start interrupt here
	//readTickCount is count round to call interrupt
	CurieTimerOne.start(time, &UpdateSensor);
	//End of interrupt

    //Send data
	footCharAx.setValue(Acc.AxSend[i]);
	footCharAy.setValue(Acc.AySend[i]);
	footCharAz.setValue(Acc.AzSend[i]);
//  footCharAx.setValue(Acc.AxStore);
//  footCharAy.setValue(Acc.AyStore);
//  footCharAz.setValue(Acc.AzStore);
  	
  	

	delay(1000);  //1 seconds of delay, regularly 'interrupted' by the timer interrupt

	//reset Tick count
  Serial.print("Count: ");
	Serial.println(CurieTimerOne.rdRstTickCount());
  Serial.print("SizeofArray: ");
  Serial.println(sizeof(Acc.AxSend));
}

void UpdateSensor()
{
	i = CurieTimerOne.readTickCount();
  Serial.println(i);

	// CurieIMU.readAccelerometerScaled(aStore->AxStore[i-1], aStore->AyStore[i-1], aStore->AzStore[i-1]);
	// #ifdef SERIAL_PORT_LOG_ENABLE
	// 	Serial.print("X:");
	// 	Serial.print(aStore->AxStore[i-1]"\t");
	// 	Serial.print("Y:");
	// 	Serial.print(aStore->AyStore[i-1]"\t");
	// 	Serial.print("Z:");
	// 	Serial.println(aStore->AzStore[i-1]);
	// #endif


CurieIMU.readAccelerometerScaled(Acc.AxStore, 
                                 Acc.AyStore, 
                                 Acc.AzStore);

	//#ifdef SERIAL_PORT_LOG_ENABLE
		Serial.print("X:");
		Serial.print(Acc.AxStore);
    Serial.print("\t");

		
		Serial.print("Y:");
		Serial.print(Acc.AyStore);
    Serial.print("\t");
	
		
		Serial.print("Z:");
		Serial.println(Acc.AzStore);

		//Copy data to A...Send[] cause sensor need A...Store to store data
		 Acc.AxSend[i] = Acc.AxStore;
		 Acc.AySend[i] = Acc.AyStore;
		 Acc.AzSend[i] = Acc.AzStore;

    

		
	
 

}

