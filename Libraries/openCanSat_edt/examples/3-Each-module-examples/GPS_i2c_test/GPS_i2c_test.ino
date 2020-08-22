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
#include "Open_Cansat_GPS.h"

#include <Wire.h>

#define Serial SerialUSB

OpenCansatGPS gps;

int ms_delay = 250; // Number of milliseconds for scan and between another scan is done

void setup()
{
    while (!SerialUSB) {}

    Serial.begin(57600);

    Serial.println("OpenCansat gps test");

    gps.begin();

    // Uncomment when you want to see debug prints from GPS library
    //gps.debugPrintOn(57600);
}

void loop()
{
	// Save start time in millisec
    uint32_t start = millis();

    // Printing gps values if scan is successful
    if (gps.scan(ms_delay))
    {
    		Serial.println(String(" time to find fix: ") + (millis() - start) + String("ms"));
    		Serial.println(String(" datetime = ") + gps.getDateTimeString());
    		Serial.println(String(" lat = ") + String(gps.getLat(), 7));
    		Serial.println(String(" lon = ") + String(gps.getLon(), 7));
    		Serial.println(String(" num sats = ") + String(gps.getNumberOfSatellites()));
    		Serial.println();
    }

    // Wait before next measurement
    delay(ms_delay);
}
