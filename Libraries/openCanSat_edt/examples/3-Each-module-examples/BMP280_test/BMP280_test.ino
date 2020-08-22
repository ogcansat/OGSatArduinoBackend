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
#include <Adafruit_BMP280.h> // include Adafruit BMP280 library

#define Serial SerialUSB // change default serial to SAMD USB serial

#define BMP280_ADDRESS 0x76 // set BMP's address (according to the circuit)

// create object 'bmp' from the library, which will  
// be used to access the library methods by a dot notation
Adafruit_BMP280 bmp; 

int ms_delay = 500; // Number of milliseconds between each two measurements

// empty variables for measured values
float temperature_bmp280 = 0;
float pressure_bmp280 = 0;
float altitude_bmp280 = 0;

// This will only run once
void setup()
{
  Serial.begin(57600); 	// Start serial comm with baud rate of 57600Bd

  // Wait for the Arduino serial (on your PC) to connect
  // please, open the Arduino serial console (right top corner)
  // note that the port may change after uploading the sketch
  // COMMENT OUT FOR USAGE WITHOUT A PC!
  while(!Serial);
  
  Serial.println("openCanSat BMP280 test started");

  // Begin communication with the BMP280 on the previously specified address
  // print an error to the serial in case the sensor is not found
  if (!bmp.begin(BMP280_ADDRESS))
  {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    return;
  }

  Serial.println();  	// Print new line on serial to make the output look better
}

// Code in this function will run repeatedly
void loop()
{   
    // Read values from BME280 into the variables
    temperature_bmp280 = bmp.readTemperature();
    pressure_bmp280 = bmp.readPressure();
    altitude_bmp280 = bmp.readAltitude(1013.25);

    // Print the measured variables to serial
    Serial.println("Temperature = " + (String) temperature_bmp280 + " *C");  
    Serial.println("Pressure = " + (String) pressure_bmp280 + " Pa");
    Serial.println("Approx altitude = " + (String) altitude_bmp280 + " m");
    Serial.println();
    
    delay(ms_delay); // Wait a while (defined at the beginning)
}
