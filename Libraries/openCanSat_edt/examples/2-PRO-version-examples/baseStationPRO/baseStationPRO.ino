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

#include <OcsStorage.h>
#include <RFM69.h>
#include <Ucglib.h>

#ifndef UCG_INTERRUPT_SAFE
#define UCG_INTERRUPT_SAFE
#endif

uint8_t screenNum = 1;

#define Serial SerialUSB

// LOCAL
#define PC_BAUDRATE     	56700

// BUTTONS
#define BUTTON_1 5
#define BUTTON_2 4
#define BUTTON_3 3

// RFM69
#define NETWORKID     	0   // Must be the same for all nodes (0 to 255)
#define MYNODEID      	2   // My node ID (0 to 255)
#define FREQUENCY   		RF69_433MHZ
#define FREQUENCYSPECIFIC 433000000  // Should be value in Hz, now 433 Mhz will be set
#define chip_select_pin 	43
#define interupt_pin		9

bool isRadioOk = true;

RFM69 radio(chip_select_pin, interupt_pin, true);
// RFM69

Ucglib_ST7735_18x128x160_HWSPI ucg(6, 7, -1);
OcsGraphics ocsDesign(ucg);
OcsStorage ocsData(ocsDesign);

OcsStorage::message income;

void setup()
{
  Serial.begin(PC_BAUDRATE);

  //while(!Serial);

  Serial.println("PpenCanSat base station test started");

  // Space behind font is transparent
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);

  // Clear the screen and reset the clip range to maximum
  ucg.clearScreen();

  ocsDesign.drawBackground();
  ocsDesign.drawHomescreen();

  // Buttons
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);

  // RFM69
  Serial.print("OpenCansat ");
  Serial.print(MYNODEID,DEC);
  Serial.println(" is ready");

  if(!radio.initialize(FREQUENCY, MYNODEID, NETWORKID))
  {
	  isRadioOk = false;
	  Serial.println("RFM69HW initialization failed!");
  }
  else
  {
    radio.setFrequency(FREQUENCYSPECIFIC);
	  radio.setHighPower(true); // Always use this for RFM69HW
  }
  //// RFM69
}

void loop()
{
  int button1 = digitalRead(BUTTON_1);
  int button2 = digitalRead(BUTTON_2);
  int button3 = digitalRead(BUTTON_3);



  if(button1 == LOW)
  {
	if(screenNum == 1)
	{
	 	screenNum = 0;
	 	ocsDesign.drawLeftScreen();
	 	delay(300);
	 	Serial.println("Button 1 ");
	}
	else if(screenNum == 2)
	{
		screenNum = 1;
	 	ocsDesign.drawHomescreen();
	 	delay(300);
	 	Serial.println("Button 1 ");
	}
  }
  else if(button2 == LOW)
  {
	if(screenNum != 1)
	{
	 	screenNum = 1;
	 	ocsDesign.drawHomescreen();
	 	delay(300);
	 	Serial.println("Button 2 ");
	}
  }
  else if(button3 == LOW)
  {
	if(screenNum == 1)
	{
		screenNum = 2;
  	 	ocsDesign.drawRightScreen();
	 	delay(300);
  	 	Serial.println("Button 3 ");
	}
	else if(screenNum == 0)
	{
		screenNum = 1;
	 	ocsDesign.drawHomescreen();
	 	delay(300);
	 	Serial.println("Button 3 ");
	}
  }  

  if (radio.receiveDone()) // Got one!
  {
	Serial.println("Message received: " + String(income.messageId));

	income = *(OcsStorage::message*)radio.DATA;
	ocsData.Update(income, screenNum);
	delay(300);
   }
}

