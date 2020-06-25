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
#include <RFM69.h> // include RFM69 library

#define Serial SerialUSB // change default serial to SAMD USB serial

// RFM69
#define NETWORKID     	  0     // Must be the same for all nodes (0 to 255)
#define MYNODEID      	  2     // My node ID (0 to 255)
#define FREQUENCY   		  RF69_433MHZ
#define FREQUENCYSPECIFIC 433000000  // Should be value in Hz, now 433 Mhz will be set
#define CHIP_SELECT_PIN	  43    //radio chip select
#define INTERUP_PIN		    9     //radio interrupt
#define MS_DELAY 		      300   // Number of milliseconds between data sending and LED signalization

// Local
#define PC_BAUDRATE     	56700

// Variables
int idCounter = 1;

RFM69 radio(CHIP_SELECT_PIN, INTERUP_PIN, true);

typedef struct
{
  int messageId;
} message;

message data; //create the struct variable

// this will only run once
void setup()
{
    Serial.begin(PC_BAUDRATE); // start serial comm with baud rate of 57600Bd

    // wait for the Arduino serial (on your PC) to connect
    // please, open the Arduino serial console (right top corner)
	// note that the port may change after uploading the sketch
    // COMMENT OUT FOR USAGE WITHOUT A PC!
    while(!Serial);

    Serial.println("openCanSat RFM69 receive test started");

    // RFM69 initialize
    radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
    radio.setFrequency(FREQUENCYSPECIFIC);
    radio.setHighPower(true); // Always use this for RFM69HW
    
    Serial.println(); // print new line on serial to make the output look better :-;
}

// code in this function will run repeatedly
void loop()
{
	// When receive from radio is done
	if (radio.receiveDone()) // Got one message
	{
		// Print node number
		Serial.println("Received from node " + static_cast<String>(radio.SENDERID));

		// transfer data into structure
		data = *(message*)radio.DATA;

		Serial.println("Id of received message: " + static_cast<String>(data.messageId));
	}
}
