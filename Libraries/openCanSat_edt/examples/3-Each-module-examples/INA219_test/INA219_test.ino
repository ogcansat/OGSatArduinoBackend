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
#include <Adafruit_INA219.h> 	// Include Adafruit INA219 library

#define Serial SerialUSB			// Change default serial to SAMD USB serial

// Create object 'ina219' from the library with address 0x40
// (according to the circuit, which will be used to access the
// library methods by a dot notation
Adafruit_INA219 ina219(0x40);

int ms_delay = 500; // Number of milliseconds between each two readings

// Empty variables for measured values
float voltage_shunt = 0;
float voltage_bus = 0;
float current_mA = 0;
float voltage_load = 0;
  
// This will only run once
void setup(void) 
{ 
  Serial.begin(57600); // start serial comm with baud rate of 57600Bd

  // Wait for the Arduino serial (on your PC) to connect
  // please, open the Arduino serial console (right top corner)
  // note that the port may change after uploading the sketch
  // COMMENT OUT FOR USAGE WITHOUT A PC!
  while(!Serial);
  
  Serial.println("openCanSat INA219 test started");

  // Begin communication with the INA219
  ina219.begin();
}

// Code in this function will run repeatedly
void loop(void)
{
  // Read values from INA219 into the variables
  voltage_shunt = ina219.getShuntVoltage_mV();
  voltage_bus = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  voltage_load = voltage_bus + (voltage_shunt / 1000);

  // Print the measured variables to serial
  Serial.println("Shunt Voltage: " + (String) voltage_shunt + " mV"); 
  Serial.println("Bus Voltage: " + (String) voltage_bus + " V");
  Serial.println("Current: " + (String) current_mA + " mA");  
  Serial.println("Load Voltage: " + (String) voltage_load + " V"); 
  Serial.println(); 
  
  delay(ms_delay); // Wait a while (defined at the beginning)
}
