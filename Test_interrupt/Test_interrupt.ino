#include <BMI160.h>
#include "CurieIMU.h"
#include <CurieIMU.h>
#include <CurieBLE.h>
#include "CurieTimerOne.h"

// Comment the following statement to disable logging on serial port.
#define SERIAL_PORT_LOG_ENABLE 1

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
	float AxSend[9];
	float AySend[9];	
	float AzSend[9];

}Accelerometer;

typedef struct Gyroscope
{
	float Gx[9];
	float Gy[9];
	float Gz[9];
	
}Gyroscope;

//float axRaw, ayRaw, azRaw;
int i = 0; // Count Element In Array 
const int time = 100000;
Accelerometer Acc;

//Accelerometer *aSend = Acc;
//Accelerometer *aStore = Acc;


void setup()
{
	#ifdef SERIAL_PORT_LOG_ENABLE
		Serial.begin(9600);
		while(!Serial){
		    // Wainting for the serial port open
		}
	#endif

	BLE.begin();

	CurieIMU.begin();

	CurieIMU.setAccelerometerRange(2);

	BLE.setLocalName("FOOT");

	//BLE.setAdvertiseService("footService");
	BLE.setAdvertisedServiceUuid(footService.uuid());
//	BLE.addAttribute(footService);
// 	BLE.addAttribute(footCharAx);
//  	BLE.addAttribute(footCharAy);
//  	BLE.addAttribute(footCharAz);
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
	BLEDevice central = BLE.central();

	//start interrupt here
	//readTickCount is count round to call interrupt
	//CurieTimerOne.start(time, &UpdateSensor);
	//Serial.println(CurieTimerOne.readTickCount());
	//End of interrupt
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
	      UpdateSensor();
      }

  }

	//Copy data to A...Send[] cause sensor need A...Store[]  to store data
//	Acc.AxSend[CurieTimerOne.readTickCount()] = Acc.AxStore[CurieTimerOne.readTickCount()];
//	Acc.AySend[CurieTimerOne.readTickCount()] = Acc.AyStore[CurieTimerOne.readTickCount()];
//	Acc.AzSend[CurieTimerOne.readTickCount()] = Acc.AzStore[CurieTimerOne.readTickCount()];
//	 #ifdef SERIAL_PORT_LOG_ENABLE
//	 	Serial.println(Acc.AxSend[CurieTimerOne.readTickCount()]);
//	 	Serial.println(Acc.AySend[CurieTimerOne.readTickCount()]);
//	 	Serial.print(Acc.AzSend[CurieTimerOne.readTickCount()]);
//	 #endif
	//Send data
//	footCharAx.setValue(Acc.AxSend[CurieTimerOne.readTickCount()]);
//	footCharAy.setValue(Acc.AySend[CurieTimerOne.readTickCount()]);
//	footCharAz.setValue(Acc.AzSend[CurieTimerOne.readTickCount()]);
  // footCharAx.setValue(Acc.AxStore[CurieTimerOne.readTickCount()]);
  // footCharAy.setValue(Acc.AyStore[CurieTimerOne.readTickCount()]);
  // footCharAz.setValue(Acc.AzStore[CurieTimerOne.readTickCount()]);
	
	

}

void UpdateSensor()
{
//	if(CurieTimerOne.readTickCount() == 11){
//		CurieTimerOne.rdRstTickCount();
//	}

	// CurieIMU.readAccelerometerScaled(aStore->AxStore[i-1], aStore->AyStore[i-1], aStore->AzStore[i-1]);
	// #ifdef SERIAL_PORT_LOG_ENABLE
	// 	Serial.print("X:");
	// 	Serial.print(aStore->AxStore[i-1]"\t");
	// 	Serial.print("Y:");
	// 	Serial.print(aStore->AyStore[i-1]"\t");
	// 	Serial.print("Z:");
	// 	Serial.println(aStore->AzStore[i-1]);
	// #endif

//	CurieIMU.readAccelerometerScaled(Acc.AxStore[CurieTimerOne.readTickCount()-1], 
//	                                 Acc.AyStore[CurieTimerOne.readTickCount()-1], 
//	                                 Acc.AzStore[CurieTimerOne.readTickCount()-1]);
CurieIMU.readAccelerometerScaled(Acc.AxStore, 
                                   Acc.AyStore, 
                                   Acc.AzStore);

	//#ifdef SERIAL_PORT_LOG_ENABLE
		Serial.print("X:");
		Serial.print(Acc.AxStore);
    Serial.print("\t");
		//Acc.AxStore[i-1] = axRaw; //Copy value to A...Store[]
		
		Serial.print("Y:");
		Serial.print(Acc.AyStore);
    Serial.print("\t");
		//Acc.AyStore[i-1] = ayRaw;
		
		Serial.print("Z:");
		Serial.println(Acc.AzStore);
		//Acc.AzStore[i-1] = azRaw;
	//#endif
  footCharAx.setValue(Acc.AxStore);
  footCharAy.setValue(Acc.AyStore);
  footCharAz.setValue(Acc.AzStore);

}

