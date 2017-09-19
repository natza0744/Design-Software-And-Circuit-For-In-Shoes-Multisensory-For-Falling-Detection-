#include <BMI160.h>
#include "CurieIMU.h"
#include <CurieIMU.h>
#include <CurieBLE.h>
#include "CurieTimerOne.h"

// Comment the following statement to disable logging on serial port.
//#define SERIAL_PORT_LOG_ENABLE 1

//BLEPeripheral blePeripheral;

BLEService footService("d7344650-9cf3-4df5-9a21-0fcbd7a35b3f");
//Accelerometer's characteristic
BLEUnsignedCharCharacteristic footCharAx("d7344651-9cf3-4df5-9a21-0fcbd7a35b3f",BLERead | BLENotify); //uuid for Ax-axis
BLEUnsignedCharCharacteristic footCharAy("d7344652-9cf3-4df5-9a21-0fcbd7a35b3f",BLERead | BLENotify); //uuid for Ay-axis
BLEUnsignedCharCharacteristic footCharAz("d7344653-9cf3-4df5-9a21-0fcbd7a35b3f",BLERead | BLENotify); //uuid for Az-axis
//Gyroscope's characteristic
BLEUnsignedCharCharacteristic footCharGx("d7344654-9cf3-4df5-9a21-0fcbd7a35b3f",BLERead | BLENotify); //uuid for Gx-axis
BLEUnsignedCharCharacteristic footCharGy("d7344655-9cf3-4df5-9a21-0fcbd7a35b3f",BLERead | BLENotify); //uuid for Gx-axis
BLEUnsignedCharCharacteristic footCharGz("d7344656-9cf3-4df5-9a21-0fcbd7a35b3f",BLERead | BLENotify); //uuid for Gx-axis
//FSR's characteristic


// number of element in array is 9 because i need to sampling 10 value in 1 second
typedef struct accelerometer
{
  float AxStore;  
  float AyStore;
  float AzStore;
  float AxSend[100];
  float AySend[100];  
  float AzSend[100];

}Accelerometer;

typedef struct gyroscope
{
  float GxStore;  
  float GyStore;
  float GzStore;
  float GxSend[100];
  float GySend[100];
  float GzSend[100];
  
}Gyroscope;

//float axRaw, ayRaw, azRaw;
int i = 0, j = 0; // Count Element In Array 
int time = 100000;
Accelerometer Acc;
Gyroscope Gyr;

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
  CurieIMU.setGyroRange(250);
  
  BLE.setLocalName("FOOT");

  //BLE.setAdvertiseService("footService");
  BLE.setAdvertisedServiceUuid(footService.uuid());

  //Add the characteristic to the service
  footService.addCharacteristic(footCharAx);
  footService.addCharacteristic(footCharAy);
  footService.addCharacteristic(footCharAz);
  footService.addCharacteristic(footCharGx);
  footService.addCharacteristic(footCharGy);
  footService.addCharacteristic(footCharGz);
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
  footCharAx.setValue(Acc.AxSend[j]);
  footCharAy.setValue(Acc.AySend[j]);
  footCharAz.setValue(Acc.AzSend[j]);
  
  footCharAx.setValue(Gyr.GxSend[j]);
  footCharAy.setValue(Gyr.GySend[j]);
  footCharAz.setValue(Gyr.GzSend[j]);
    
    

  delay(1000);  //1 seconds of delay, regularly 'interrupted' by the timer interrupt

  //reset Tick count
  //Serial.print("Count: ");
  //Serial.println(CurieTimerOne.rdRstTickCount());
  Serial.print("SizeofArray: ");
  Serial.println(sizeof(Acc.AxSend));
//  if (i = 100){
//    Serial.println(CurieTimerOne.rdRstTickCount());
//  }
  
}
void UpdateSensor()
{
  i = CurieTimerOne.readTickCount();
  Serial.println(i);

  //Reset j cause j will count element in array and the array have 100 element for store data 10Second
  if(j == 100){
    j = 0;
  }
  // CurieIMU.readAccelerometerScaled(aStore->AxStore[i-1], aStore->AyStore[i-1], aStore->AzStore[i-1]);
  // #ifdef SERIAL_PORT_LOG_ENABLE
  //  Serial.print("X:");
  //  Serial.print(aStore->AxStore[i-1]"\t");
  //  Serial.print("Y:");
  //  Serial.print(aStore->AyStore[i-1]"\t");
  //  Serial.print("Z:");
  //  Serial.println(aStore->AzStore[i-1]);
  // #endif


    CurieIMU.readAccelerometerScaled(Acc.AxStore, 
                                     Acc.AyStore, 
                                     Acc.AzStore);

    //#ifdef SERIAL_PORT_LOG_ENABLE
    Serial.print("AX[");
    Serial.print(j);
    Serial.print("]:");
    Serial.print(Acc.AxStore);
    //Serial.print("\t");

    
    Serial.print("  AY:");
    Serial.print(Acc.AyStore);
    //Serial.print("\t");
  
    
    Serial.print("  AZ:");
    Serial.println(Acc.AzStore);

    //Copy data to A...Send[] cause sensor need A...Store to store data
     Acc.AxSend[j] = Acc.AxStore;
     Acc.AySend[j] = Acc.AyStore;
     Acc.AzSend[j] = Acc.AzStore;

    CurieIMU.readGyroScaled(Gyr.GxStore, Gyr.GyStore, Gyr.GzStore);

    Serial.print("GX:");
    Serial.print(Gyr.GxStore);
    //Serial.print("\t");

    
    Serial.print("  GY:");
    Serial.print(Gyr.GyStore);
    //Serial.print("\t");
  
    
    Serial.print("  GZ:");
    Serial.println(Gyr.GzStore);

    Gyr.GxSend[j] = Gyr.GxStore;
    Gyr.GySend[j] = Gyr.GyStore;
    Gyr.GzSend[j] = Gyr.GzStore;

    Serial.print("Az[j]: ");
    Serial.println(Acc.AzSend[j-1]);

    j++;
}

