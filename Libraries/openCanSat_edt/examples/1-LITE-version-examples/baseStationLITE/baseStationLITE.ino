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
#include <RFM69.h>

#define Serial SerialUSB

/// RFM69 variables
#define NETWORKID     	0   // Must be the same for all nodes (0 to 255)
#define MYNODEID      	2   // My node ID (0 to 255)
#define FREQUENCY   		RF69_433MHZ
#define FREQUENCYSPECIFIC 433000000  // Should be value in Hz, now 433 Mhz will be set
#define chip_select_pin 	43
#define interupt_pin		9

RFM69 radio(chip_select_pin, interupt_pin, true);

bool isRadioOk = true;

/*
 * Structure for CanSat lite
 */
typedef struct
{
  uint16_t messageId;
  float temperature;
  float pressure;
  float altitude;
  int16_t rssi;
} message;

// Income message data storage
message incomeMessage = {0, 0, 0, 0, 0};

void setup()
{
  Serial.begin(57600); // Start serial comm with baud rate of 57600Bd

  // Wait for the Arduino serial (on your PC) to connect
  // please, open the Arduino serial console (right top corner)
  // note that the port may change after uploading the sketch
  // COMMENT OUT FOR USAGE WITHOUT A PC!
  while(!Serial);

  Serial.println("Base station LITE started");
  Serial.print("Node " + static_cast<String>(MYNODEID) + " ready");

  // Radio RFM69 init (Frequency, node id and network id needed)
  if(!radio.initialize(FREQUENCY, MYNODEID, NETWORKID))
  {
	  isRadioOk = false;		// radio init failed flag
	  Serial.println("Could not find a valid RFM69HW sensor, check wiring!");
  }
  else
  {
    radio.setFrequency(FREQUENCYSPECIFIC);
	  radio.setHighPower(true); // Always use this for RFM69HW
  }
}

void loop()
{
	// If radio received message print out received data
	if (radio.receiveDone())
	{
		Serial.println("Received from canSat number " + static_cast<String>(radio.SENDERID));

		// Convert income data into data structure
		incomeMessage = *(message*)radio.DATA;

		// Print out received data
		Serial.println("MessageId = " + static_cast<String>(incomeMessage.messageId));
		Serial.println("Temperature = " + static_cast<String>(incomeMessage.temperature) + " *C");
		Serial.println("Pressure = " + static_cast<String>(incomeMessage.pressure) + " Pa");
		Serial.println("Approx altitude = " + static_cast<String>(incomeMessage.altitude) + " m");
		Serial.println("Rssi signal = " + static_cast<String>(incomeMessage.rssi));
		Serial.println();
	}
}


