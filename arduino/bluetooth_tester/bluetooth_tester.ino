
/*
  LED

  This example creates a BLE peripheral with service that contains a
  characteristic to control an LED.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  You can use a generic BLE central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

#include "AS726X.h"
#include <math.h>



AS726X sensor;

BLEService ledService("180F"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
//BLECharCharacteristic switchCharacteristic("2A57", BLERead | BLEWrite);
BLECharacteristic sensorR("2A58", BLERead | BLEWrite, "SENSOR_R");
BLECharacteristic sensorS("2A59", BLERead | BLEWrite, "SENSOR_S");
BLECharacteristic sensorT("2A60", BLERead | BLEWrite, "SENSOR_T");
BLECharacteristic sensorU("2A61", BLERead | BLEWrite, "SENSOR_U");
BLECharacteristic sensorV("2A62", BLERead | BLEWrite, "SENSOR_V");
BLECharacteristic sensorW("2A58", BLERead | BLEWrite, "SENSOR_W");

const int ledPin = LED_BUILTIN; // pin to use for the LED


void setup() {
  Wire.begin();
  //Serial.begin(115200);

  sensor.begin();
  //while (!Serial);

  // set LED pin to output mode
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(D3, INPUT_PULLUP);

  digitalWrite(LED_BUILTIN, LOW);         // when the central disconnects, turn off the LED
  digitalWrite(LEDR, HIGH);          // will turn the LED off
  digitalWrite(LEDG, HIGH);        // will turn the LED off
  digitalWrite(LEDB, HIGH); 

  //set up json document 
  
  // begin initialization
  if (!BLE.begin()) {
    //Serial.println("starting BLE failed!");
    
    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Nano 33 BLE");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
//  ledService.addCharacteristic(switchCharacteristic);
  ledService.addCharacteristic(sensorR);
  ledService.addCharacteristic(sensorS);
  ledService.addCharacteristic(sensorT);
  ledService.addCharacteristic(sensorU);
  ledService.addCharacteristic(sensorV);
  ledService.addCharacteristic(sensorW);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
 // switchCharacteristic.writeValue(0);
 /*
  char buf[16];
  sensorR.writeValue(snprintf(buf, 15, "%d", sensor.getCalibratedR()), );
  sensorS.writeValue(snprintf(buf, 15, "%d", sensor.getCalibratedS()));
  sensorT.writeValue(snprintf(buf, 15, "%d", sensor.getCalibratedT()));
  sensorU.writeValue(snprintf(buf, 15, "%d", sensor.getCalibratedU()));
  sensorV.writeValue(snprintf(buf, 15, "%d", sensor.getCalibratedV()));
  sensorW.writeValue(snprintf(buf, 15, "%d", sensor.getCalibratedW()));
  */

  // start advertising
  BLE.advertise();

  //Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    //Serial.print("Connected to central: ");
    // print the central's MAC address:
    //Serial.println(central.address());
   

    // while the central is still connected to peripheral:
    while (central.connected()) {
      
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      //if (switchCharacteristic.written()) {
        //switch (switchCharacteristic.value()) {   // any value other than 0
          //case 01:
           // Serial.println("Red LED on");
           if (digitalRead(D3) == LOW){
              digitalWrite(LEDR, HIGH);            // will turn the LED on
              digitalWrite(LEDG, LOW);         // will turn the LED off
              digitalWrite(LEDB, HIGH);         // will turn the LED off
              sensor.takeMeasurementsWithBulb();
              if (sensor.getVersion() == SENSORTYPE_AS7263)
              {
                //Near IR readings
                char buf[7];
                String Rstring = String(sensor.getCalibratedR(), 2);
                Rstring.toCharArray(buf, 7);
                sensorR.writeValue((unsigned char*)buf, 7);
                
                String Sstring = String(sensor.getCalibratedS(), 2);
                Sstring.toCharArray(buf, 7);
                sensorS.writeValue((unsigned char*)buf, 7);
  
                String Tstring = String(sensor.getCalibratedT(), 2);
                Tstring.toCharArray(buf, 7);
                sensorT.writeValue((unsigned char*)buf, 7);
  
                String Ustring = String(sensor.getCalibratedU(), 2);
                Ustring.toCharArray(buf, 7);
                sensorU.writeValue((unsigned char*)buf, 7);
  
                String Vstring = String(sensor.getCalibratedV(), 2);
                Vstring.toCharArray(buf, 7);
                sensorV.writeValue((unsigned char*)buf, 7);
  
                String Wstring = String(sensor.getCalibratedW(), 2);
                Wstring.toCharArray(buf, 7);
                sensorW.writeValue((unsigned char*)buf, 7);
                
              }
              delay(5);
              while (digitalRead(D3) == LOW);     
      }
      
    }

    // when the central disconnects, print it out:
    //Serial.print(F("Disconnected from central: "));
    //Serial.println(central.address());
    //Serial.println(central.address());
    digitalWrite(LED_BUILTIN, LOW);         // when the central disconnects, turn off the LED
    digitalWrite(LEDR, HIGH);          // will turn the LED off
    digitalWrite(LEDG, HIGH);        // will turn the LED off
    digitalWrite(LEDB, HIGH);
  }
}
