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
#include "Adafruit_BME280.h" 			// Include Adafruit BME280 library

#define Serial SerialUSB 				// Change default serial to SAMD USB serial

#define BME280_ADDRESS_OPEN_CANSAT 0x77 	// Set BME's address (according to the circuit)
#define SEALEVELPRESSURE_HPA (1013.25) 	// Set sea level pressure value(see: https://en.wikipedia.org/wiki/Atmospheric_pressure)

// Create object 'bme' from the library, which will
// be used to access the library methods by a dot notation
Adafruit_BME280 bme;

int ms_delay = 500; 			// Number of milliseconds between each two measurements

// Empty variables for measured values
float temperature_bme280 = 0;
float pressure_bme280 = 0;
float altitude_bme280 = 0;
float humidity_bme280 = 0;

// This will only run once
void setup()
{
    Serial.begin(57600); 	// Start serial comm with baud rate of 57600Bd

    // Wait for the Arduino serial (on your PC) to connect
    // please, open the Arduino serial console (right top corner)
	// note that the port may change after uploading the sketch
    // COMMENT OUT FOR USAGE WITHOUT A PC!
    while(!Serial);

    Serial.println("openCanSat BME280 test started");

    // Begin communication with the BME280 on the previously specified address
    // print an error to the serial in case the sensor is not found
    if (!bme.begin(BME280_ADDRESS_OPEN_CANSAT))
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        return;
    }
    
    Serial.println(); 		// Print new line on serial to make the output look better :-;
}

// Code in this function will run repeatedly
void loop()
{ 
  // Read values from BME280 into the variables
  temperature_bme280 = bme.readTemperature();
  pressure_bme280 = bme.readPressure() / 100.0F;
  altitude_bme280 = bme.readAltitude(SEALEVELPRESSURE_HPA);
  humidity_bme280 = bme.readHumidity();

  // Print the measured variables to serial
  Serial.println("Temperature = " + (String) temperature_bme280 + " °C");  
  Serial.println("Pressure = " + (String) pressure_bme280 + " hPa");
  Serial.println("Approx altitude = " + (String) altitude_bme280 + " m");
  Serial.println("Humidity = " + (String) humidity_bme280 + " %");
  Serial.println();
    
  delay(ms_delay); // Wait a while (defined at the beginning)
}
