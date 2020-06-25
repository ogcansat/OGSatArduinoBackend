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
#include <Arduino.h>

#define Serial SerialUSB 	// Change default serial to SAMD USB serial

// Set LED pins
#define D13_LED 42
#define M_LED 36

int ms_delay = 2000;  		// Number of milliseconds between each two LED changes

void setup() 
{
  Serial.begin(57600); 		// Start serial comm with baud rate of 57600Bd

  // Wait for the Arduino serial (on your PC) to connect
  // please, open the Arduino serial console (right top corner)
  // note that the port may change after uploading the sketch
  // COMMENT OUT FOR USAGE WITHOUT A PC!
  while(!Serial);
  
  Serial.println("openCanSat kit LED test started");

  pinMode(D13_LED, OUTPUT); 	// Configure D13 pin as output
}

void loop() 
{
  pinMode(M_LED, OUTPUT); 	// Configure MLED as output (it is not, after every loop)
  
  // Print the LEDs' status to serial
  Serial.println("D13: ON");
  Serial.println("MLED: ON, RED");
  Serial.println();

  // Turn D13 and MLED-red on
  digitalWrite(D13_LED, HIGH);   
  digitalWrite(M_LED, HIGH); 

  delay(ms_delay); 			// Wait a while (defined at the beginning)

  // Print the LEDs' status to serial
  Serial.println("D13: OFF");
  Serial.println("MLED: ON, BLUE");
  Serial.println();  

  // Turn D13 off and MLED-blue on
  digitalWrite(D13_LED, LOW);   
  digitalWrite(M_LED, LOW);  

  delay(ms_delay); 			// Wait a while (defined at the beginning)

  // Print the LEDs' status to serial
  Serial.println("D13: OFF");
  Serial.println("MLED: OFF");
  Serial.println();

  // D13 is already off
  pinMode(M_LED, INPUT); 	// Configure MLED as input to turn off voltage supply on the pin
  
  delay(ms_delay); 			// Wait a while (defined at the beginning)
}
