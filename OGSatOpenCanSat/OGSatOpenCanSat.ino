/*
  This code was designed specifically for usage with the openCanSat kit (https://opencansat.eu)

  This program is free software; you can redistribute it
  and/or modify it under the terms of the GNU General
  Public License as published by the Free Software
  Foundation; either version 3 of the License, or
  (at your option) any later version.

  You should have received a copy of the GNU General
  Public License along with this program.
  If not, see <http://www.gnu.org/licenses/>.
  Licence can be viewed at
  http://www.gnu.org/licenses/gpl-3.0.txt
  Please maintain this license information along with authorship
  and copyright notices in any redistribution of this code
 */

#include "Arduino.h"

#include <Adafruit_BME280.h>  // include Adafruit BME280 library
#include <Adafruit_INA219.h>  // include INA219
#include <SD.h> 				 // include Arduino SD library
#include "Open_Cansat_GPS.h"

#include <RFM69.h>			 // include RFM69 library
#include <SPI.h>
#include <Servo.h>

// Local
#define PC_BAUDRATE     	56700
#define MS_DELAY 		0  // Number of milliseconds between data sending and LED signalization
#define LED_DELAY 		100
#define Serial SerialUSB

// RFM69
#define NETWORKID     	0  		   // Must be the same for all nodes (0 to 255)
#define MYNODEID      	1   		   // My node ID (0 to 255)
#define TONODEID      	2   		   // Destination node ID (0 to 254, 255 = broadcast)
#define FREQUENCY     	RF69_433MHZ   // Frequency set up
#define FREQUENCYSPECIFIC 434300000  // Should be value in Hz, now 433 Mhz will be set
#define CHIP_SELECT_PIN 	43 //radio chip select
#define INTERUP_PIN     	9 //radio interrupt

// BME280 SETTING
#define BME280_ADDRESS_OPEN_CANSAT 0x77
#define SEALEVELPRESSURE_HPA 	  1013.25

// SD card
#define sd_cs_pin 35 // set SD's chip select pin (according to the circuit)

// create object 'rf69' from the library, which will
// be used to access the library methods by a dot notation
RFM69 radio(CHIP_SELECT_PIN, INTERUP_PIN, true);

// define our own struct data type with variables; used to send data
typedef struct
{
  int16_t messageId;
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t sec;
  float longitude;
  float latitude;
  uint8_t num_of_satelites;
  float temperature;
  float pressure;
  float altitude;
  float humidity_bme280;
  float voltage_shunt;
  float voltage_bus;
  float current_mA;
  float voltage_load;
  int16_t rssi;
} messageOut;

messageOut data; //create the struct variable

// create object 'bme' from the library, which will
// be used to access the library methods by a dot notation
Adafruit_BME280 bme;

// create object 'ina219' from the library with address 0x40
// (according to the circuit, which will be used to access the
// library methods by a dot notation
Adafruit_INA219 ina219(0x40);

// create object 'gps' from the library
OpenCansatGPS gps;

// SD card file
File file; // SD library variable

// LEDS
#define D13_led_pin 42 	// D13 LED
#define M_led_pin 36 	// MLED

// Local variables
int idCounter = 1;
bool isBmeOk = true;
bool isSdOk = true;
bool isRadioOk = true;
bool isGpsConnected = true;

Servo myservo;
int pos = 0;

void setup()
{
  Serial.begin(PC_BAUDRATE);

  // wait for the Arduino serial (on your PC) to connect
  // please, open the Arduino serial console (right top corner)
  // note that the port may change after uploading the sketch
  // COMMENT OUT FOR USAGE WITHOUT A PC!
  // while(!Serial);

  Serial.println("openCanSat PRO");

  Serial.print("Node ");
  Serial.print(MYNODEID,DEC);
  Serial.println(" ready");
  
  // begin communication with the BME280 on the previously specified address
  // print an error to the serial in case the sensor is not found
  if (!bme.begin(BME280_ADDRESS_OPEN_CANSAT))
  {
	  isBmeOk = false;
	  Serial.println("Could not find a valid BME280 sensor, check wiring!");
	  return;
  }

  // begin communication with the INA219
  ina219.begin();

  // check of Gps is connected
  Wire.beginTransmission(0x42); // 42 is addres of GPS
  int error = Wire.endTransmission();

  if (error != 0)
  {
	  isGpsConnected = false;
  }

  // begin communication with gps
  gps.begin();

  // Uncomment when you want to see debug prints from GPS library
  // gps.debugPrintOn(57600);

  if(!radio.initialize(FREQUENCY, MYNODEID, NETWORKID))
  {
	  isRadioOk = false;
	  Serial.println("RFM69HW initialization failed!");
  }
  else
  {
    radio.setFrequency(FREQUENCYSPECIFIC);
	  radio.setHighPower(true); // Always use this for RFM69HW
  }

  pinMode(D13_led_pin, OUTPUT);

  myservo.attach(A0);
}

void loop()
{
  data.messageId = idCounter;

  Serial.println("MessageId = " + static_cast<String>(data.messageId));

  data.temperature = 0;
  data.pressure = 0;
  data.altitude = 0;

  if(isBmeOk)
  {
	  data.temperature += bme.readTemperature();
	  data.pressure += bme.readPressure() / 100.0F;
	  data.altitude += bme.readAltitude(SEALEVELPRESSURE_HPA);
	  data.humidity_bme280 = bme.readHumidity();
  }

  Serial.println("Temperature = " + static_cast<String>(data.temperature) + " *C");
  Serial.println("Pressure = " + static_cast<String>(data.pressure) + " Pa");
  Serial.println("Approx altitude = " + static_cast<String>(data.altitude) + " m");
  Serial.println("Humidity = " + static_cast<String>(data.humidity_bme280) + " %");

  // read values from INA219 into structure
  data.voltage_shunt = ina219.getShuntVoltage_mV();
  data.voltage_bus = ina219.getBusVoltage_V();
  data.current_mA = ina219.getCurrent_mA();
  data.voltage_load = data.voltage_bus + (data.voltage_shunt / 1000);

  Serial.println("Shunt Voltage: " + static_cast<String>(data.voltage_shunt) + " mV");
  Serial.println("Bus Voltage: " + static_cast<String>(data.voltage_bus) + " V");
  Serial.println("Current: " + static_cast<String>(data.current_mA) + " mA");
  Serial.println("Load Voltage: " + static_cast<String>(data.voltage_load) + " V");

  // Initialize GPS
  data.year = 0;
  data.month = 0;
  data.day = 0;
  data.hour = 0;
  data.minute = 0;
  data.sec = 0;
  data.latitude = 0;
  data.longitude = 0;
  data.num_of_satelites = 0;

  // save start time in millisec
  uint32_t start = millis();

  // END LED BLINK
  digitalWrite(D13_led_pin, LOW);

  pinMode(M_led_pin, INPUT);
  // END LED BLINK

  if(isGpsConnected)
  {
	  if (gps.scan(250))
	  {
		  data.year = gps.getYear();
		  data.month = gps.getMonth();
		  data.day = gps.getDay();
		  data.hour = gps.getHour();
		  data.minute = gps.getMinute();
		  data.sec = gps.getSecond();
		  data.latitude = gps.getLat();
		  data.longitude = gps.getLon();
		  data.num_of_satelites = gps.getNumberOfSatellites();
		  Serial.println(String("Time to find fix: ") + (millis() - start) + String("ms"));
		  Serial.println(String("Datetime: ") + String(data.year) + "/"+ String(data.month) + "/"+ String(data.day) + " " + String(data.hour) + ":"+ String(data.minute) + ":"+ String(data.sec));
		  Serial.println(String("Lat: ") + String(data.latitude, 7));
		  Serial.println(String("Lon: ") + String(data.longitude, 7));
		  Serial.println(String("Num of sats: ") + String(data.num_of_satelites));
		  Serial.println();
	  }
	  else
	  {
		  Serial.println("Gps have no satelit to fix.");
	  }
  }

  // RFM69HW
  data.rssi = 0;

  if(isRadioOk)
  {
	  data.rssi = radio.RSSI;
	  Serial.println("Signal = " + static_cast<String>(radio.RSSI));

	  radio.send(TONODEID, (const void*)&data, sizeof(data));
  }

  Serial.println();

  // START LED hart beat
  pinMode(M_led_pin, OUTPUT);

  digitalWrite(D13_led_pin, HIGH);

  digitalWrite(M_led_pin, HIGH);
  // START LED hart beat

  if(!isGpsConnected)
  {
	delay(200);
  }

  idCounter ++;

  for(pos = 0; pos <= 180; pos += 1) //je od úhlu 0 do úhlu 180
  {
    myservo.write(pos);  //natočení motoru na aktuální úhel
    delay(15);           //chvilka čekání než se motor natočí
  }
}
