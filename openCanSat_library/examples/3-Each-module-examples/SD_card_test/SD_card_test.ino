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
#include <SD.h> 			// Include Arduino SD library
#include <SPI.h> 		// Include Arduino SPI library

#define Serial SerialUSB // Change default serial to SAMD USB serial
#define cs_pin 35 		// Set SD's chip select pin (according to the circuit)
File file; 				// SD library variable

// this will only run once
void setup()
{
  Serial.begin(57600); // Start serial comm with baud rate of 57600Bd

  // Wait for the Arduino serial (on your PC) to connect
  // please, open the Arduino serial console (right top corner)
  // note that the port may change after uploading the sketch
  // COMMENT OUT FOR USAGE WITHOUT A PC!
  while(!Serial);
  
  Serial.println("openCanSat SD card test started");

  // Begin communication with the SD card using the previously specified pin
  // print an error to the serial in case the SD card is not found
  if (!SD.begin(cs_pin))
  {
    Serial.println("SD card initialization failed!");
    return;
  }
  
  Serial.println("SD card initialized");

  file = SD.open("data.txt", FILE_WRITE); // Open test.txt for write

  // Rrite to the file and print info to serial
  // print an error to the serial in case it does not succeed
  if (file) 
  {
    Serial.println("Writing to data.txt");
    file.println("openCanSat SD card test");
    file.close();
    
    Serial.println("Writing to file finished.");
  } 
  else 
  {
    Serial.println("Error opening data.txt for writing");
  }

  // Read the test.txt and print it to serial
  // print an error to the serial in case it does not succeed
  file = SD.open("data.txt");
  if (file)
  {
    Serial.println("data.txt :");

    while (file.available()) 
    {
      Serial.write(file.read());
    }
    file.close();
    Serial.println("Reading from data.txt finished");
  } 
  else 
  {
    Serial.println("Error to open data.txt");
  }
}

// No need for this function
void loop()
{
}
