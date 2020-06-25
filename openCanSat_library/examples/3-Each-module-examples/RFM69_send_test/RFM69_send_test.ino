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
#include <RFM69.h> 				// Include RFM69 library

#define Serial SerialUSB 		// Change default serial to SAMD USB serial

// RFM69
#define NETWORKID         0   		   // Must be the same for all nodes (0 to 255)
#define MYNODEID          1   		   // My node ID (0 to 255)
#define TONODEID          2   		   // Destination node ID (0 to 254, 255 = broadcast)
#define FREQUENCY         RF69_433MHZ
#define FREQUENCYSPECIFIC 433000000  // Should be value in Hz, now 433 Mhz will be set
#define CHIP_SELECT_PIN   43 		     // Radio chip select
#define INTERUP_PIN       9 		     // Radio interrupt
#define MS_DELAY          300 		   // Number of milliseconds between data sending and LED signalization

// Local
#define PC_BAUDRATE       56700

// Variables
int idCounter = 1;

RFM69 radio(CHIP_SELECT_PIN, INTERUP_PIN, true, INTERUP_PIN);

typedef struct
{
  int messageId;
} message;

message data; // Create the struct object

// This setup loop will only run once
void setup()
{
    Serial.begin(PC_BAUDRATE); // Start serial comm with baud rate of 57600Bd

    // Wait for the Arduino serial (on your PC) to connect
    // please, open the Arduino serial console (right top corner)
    // note that the port may change after uploading the sketch
    // COMMENT OUT FOR USAGE WITHOUT A PC!
    while(!Serial);

    Serial.println("openCanSat RFM69 receive test started");

    if(radio.initialize(FREQUENCY, MYNODEID, NETWORKID))
    {
        radio.setFrequency(FREQUENCYSPECIFIC);
    		radio.setHighPower(true); // Always use this for RFM69HW
    }
    else
    {
    		Serial.println("RFM69HW initialization failed!");
    }
    
    Serial.println(); // Print new line on serial to make the output look better :-;
}

// Code in this function will run repeatedly
void loop()
{
  // Read values from BME280 into the variables
  data.messageId = idCounter;

  // Print the id of message to serial
  Serial.println("Id of sended message: " +  static_cast<String>(idCounter));
  Serial.println();
    
  // Send data to node (with TONODEID)
  radio.send(TONODEID, (const void*)&data, sizeof(data));

  // Increase id counter
  idCounter++;

  delay(MS_DELAY); // Wait a while (defined at the beginning)
}
