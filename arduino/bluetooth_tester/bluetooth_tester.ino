#include <ArduinoJson.h>



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
BLEByteCharacteristic switchCharacteristic("2A57", BLERead | BLEWrite);
BLEByteCharacteristic sensorR("2A58", BLERead | BLEWrite);
BLEByteCharacteristic sensorS("2A59", BLERead | BLEWrite);
BLEByteCharacteristic sensorT("2A60", BLERead | BLEWrite);
BLEByteCharacteristic sensorU("2A61", BLERead | BLEWrite);
BLEByteCharacteristic sensorV("2A62", BLERead | BLEWrite);
BLEByteCharacteristic sensorW("2A58", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN; // pin to use for the LED

StaticJsonDocument<200> sensor_data;

void ftoa(float, char*, int); //initialize function

void setup() {
  Wire.begin();
  Serial.begin(115200);

  sensor.begin();
  while (!Serial);

  // set LED pin to output mode
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);         // when the central disconnects, turn off the LED
  digitalWrite(LEDR, HIGH);          // will turn the LED off
  digitalWrite(LEDG, HIGH);        // will turn the LED off
  digitalWrite(LEDB, HIGH); 

  //set up json document 
  
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    
    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Nano 33 BLE");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);
  ledService.addCharacteristic(sensorR);
  ledService.addCharacteristic(sensorS);
  ledService.addCharacteristic(sensorT);
  ledService.addCharacteristic(sensorU);
  ledService.addCharacteristic(sensorV);
  ledService.addCharacteristic(sensorW);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH);

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        switch (switchCharacteristic.value()) {   // any value other than 0
          case 01:
            Serial.println("Red LED on");
            digitalWrite(LEDR, LOW);            // will turn the LED on
            digitalWrite(LEDG, HIGH);         // will turn the LED off
            digitalWrite(LEDB, HIGH);         // will turn the LED off
            sensor.takeMeasurementsWithBulb();
            if (sensor.getVersion() == SENSORTYPE_AS7263)
            {
              //Near IR readings
              /*
              sensor_data["r"] = String(sensor.getCalibratedR(), 2);
              sensor_data["s"] = String(sensor.getCalibratedS(), 2);
              sensor_data["t"] = String(sensor.getCalibratedT(), 2);
              sensor_data["u"] = String(sensor.getCalibratedU(), 2);
              sensor_data["v"] = String(sensor.getCalibratedV(), 2);
              sensor_data["w"] = String(sensor.getCalibratedW(), 2);
              serializeJson(sensor_data, please);
              */
              sensorR.writeValue(sensor.getCalibratedR());
              sensorS.writeValue(sensor.getCalibratedS());
              sensorT.writeValue(sensor.getCalibratedT());
              sensorU.writeValue(sensor.getCalibratedU());
              sensorV.writeValue(sensor.getCalibratedV());
              sensorW.writeValue(sensor.getCalibratedW());
            }
            break;
          case 02:
            Serial.println("Green LED on");
            digitalWrite(LEDR, HIGH);         // will turn the LED off
            digitalWrite(LEDG, LOW);        // will turn the LED on
            digitalWrite(LEDB, HIGH);        // will turn the LED off
            break;
          case 03:
            Serial.println("Blue LED on");
            digitalWrite(LEDR, HIGH);         // will turn the LED off
            digitalWrite(LEDG, HIGH);       // will turn the LED off
            digitalWrite(LEDB, LOW);         // will turn the LED on
            break;
          default:
            Serial.println(F("LEDs off"));
            digitalWrite(LEDR, HIGH);          // will turn the LED off
            digitalWrite(LEDG, HIGH);        // will turn the LED off
            digitalWrite(LEDB, HIGH);         // will turn the LED off
            break;
           }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, LOW);         // when the central disconnects, turn off the LED
    digitalWrite(LEDR, HIGH);          // will turn the LED off
    digitalWrite(LEDG, HIGH);        // will turn the LED off
    digitalWrite(LEDB, HIGH);
  }
}
