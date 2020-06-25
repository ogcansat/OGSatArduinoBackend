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
#include <Adafruit_BMP280.h>	// include Adafruit BMP280 library
#include <RFM69.h>			// include RFM69 library

// Local constants
#define PC_BAUDRATE		56700
#define MS_DELAY			1500  // Number of milliseconds between data sending and LED signalization
#define Serial SerialUSB

// RFM69 constants
#define NETWORKID		0	// Must be the same for all nodes (0 to 255)
#define MYNODEID			1	// My node ID (0 to 255)
#define TONODEID			2	// Destination node ID (0 to 254, 255 = broadcast)
#define FREQUENCY		RF69_433MHZ
#define FREQUENCYSPECIFIC 433000000  // Should be value in Hz, now 433 Mhz will be set
#define CHIP_SELECT_PIN	43	//radio chip select
#define INTERUP_PIN		9	//radio interrupt

// BMP280 constants
#define BMP280_ADDRESS_OPEN_CANSAT	0x76
#define SEALEVELPRESSURE_HPA 	  1013.25

// SD card constants
#define sd_cs_pin	35	// set SD's chip select pin (according to the circuit)

// create object 'rf69' from the library, which will
// be used to access the library methods by a dot notation
RFM69 radio(CHIP_SELECT_PIN, INTERUP_PIN, true, INTERUP_PIN);

// define our own struct data type with variables; used to send data
typedef struct
{
	uint16_t messageId;
	float temperature;
	float pressure;
	float altitude;
	int16_t rssi;
} message;

message data;	//create the struct variable

// create object 'bmp' from the library, which will
// be used to access the library methods by a dot notation
Adafruit_BMP280 bmp;

// LEDS variables
#define D13_led_pin 42	// D13 LED
#define M_led_pin 36		// MLED

// Local variables
int idCounter = 1;
bool isBmpOk = true;
bool isRadioOk = true;

void setup()
{
  Serial.begin(PC_BAUDRATE);

  // wait for the Arduino serial (on your PC) to connect
  // please, open the Arduino serial console (right top corner)
  // note that the port may change after uploading the sketch
  // COMMENT OUT FOR USAGE WITHOUT A PC!
  while(!Serial);

  Serial.println("CanSat LITE started");
  Serial.println("CanSat number " + static_cast<String>(MYNODEID) + " ready.");


  // begin communication with the BMP280 on the previously specified address
  // print an error to the serial in case the sensor is not found
  if (!bmp.begin(BMP280_ADDRESS_OPEN_CANSAT))
  {
	isBmpOk = false;
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    return;
  }

  if(!radio.initialize(FREQUENCY, MYNODEID, NETWORKID))
  {
	  isRadioOk = false;
	  Serial.println("Could not find a valid RFM69HW sensor, check wiring!");
      return;
  }

  if(isRadioOk)
  {
	  Serial.println("High power was set (needed for RFM69HW).");
    radio.setFrequency(FREQUENCYSPECIFIC);
	  radio.setHighPower(true); // Always use this for RFM69HW
  }

  pinMode(D13_led_pin, OUTPUT);
}

void loop()
{
  data.messageId = idCounter;

  Serial.println("MessageId = " + static_cast<String>(data.messageId));

  data.temperature = 0;
  data.pressure = 0;
  data.altitude = 0;

  if(isBmpOk)
  {
	  data.temperature += bmp.readTemperature();
	  data.pressure += bmp.readPressure();
	  data.altitude += bmp.readAltitude(SEALEVELPRESSURE_HPA);
  }

  Serial.println("Temperature = " + static_cast<String>(data.temperature) + " *C");
  Serial.println("Pressure = " + static_cast<String>(data.pressure) + " Pa");
  Serial.println("Approx altitude = " + static_cast<String>(data.altitude) + " m");

  // RFM69HW
  data.rssi = 0;

  if(isRadioOk)
  {
	  data.rssi = radio.RSSI;
	  Serial.println("Signal = " + static_cast<String>(radio.RSSI));

	  radio.send(TONODEID, (const void*)&data, sizeof(data));

	  Serial.println("Data was sended to base station num: " + static_cast<String>(TONODEID));
  }

  Serial.println();

  // Led hart beat.
  pinMode(M_led_pin, OUTPUT);
  digitalWrite(M_led_pin, HIGH);

  delay(MS_DELAY);

  pinMode(M_led_pin, INPUT);

  // Increase message id
  idCounter ++;

  // Wait before another measurement and set data
  delay(MS_DELAY);
}
