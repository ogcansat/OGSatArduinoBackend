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

#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>



#ifndef UCG_INTERRUPT_SAFE
#define UCG_INTERRUPT_SAFE
#endif

#define Serial SerialUSB

#define BME280_ADDRESS 0x76

// LOCAL
#define PC_BAUDRATE     	9600

// BUTTONS
#define BUTTON_1 5
#define BUTTON_2 4
#define BUTTON_3 3

// RFM69
#define NETWORKID     	0   // Must be the same for all nodes (0 to 255)
#define MYNODEID      	2   // My node ID (0 to 255)
#define FREQUENCY   		RF69_433MHZ
#define FREQUENCYSPECIFIC 434300000  // Should be value in Hz, now 433 Mhz will be set
#define chip_select_pin 	43
#define interupt_pin		9

// My pins
#define sound A0
#define voltage 2
#define hum_soil A1

bool isRadioOk = true;

RFM69 radio(chip_select_pin, interupt_pin, true);
// RFM69

Ucglib_ST7735_18x128x160_HWSPI ucg(6, 7, -1);
OcsGraphics ocsDesign(ucg);
OcsStorage ocsData(ocsDesign);

OcsStorage::message income;

BH1750 lightMeter;
Adafruit_BME280 bme;

uint8_t screenNum = 1;

unsigned long time_last = 0;



void setup()
{
  
    Serial.begin(PC_BAUDRATE);

    //while(!Serial);

    //Serial.println("PpenCanSat base station test started");

    // Space behind font is transparent
    ucg.begin(UCG_FONT_MODE_TRANSPARENT);
    bme.begin(BME280_ADDRESS);
    lightMeter.begin();

    // Clear the screen and reset the clip range to maximum
    ucg.clearScreen();

    ocsDesign.drawBackground();
    ocsDesign.drawHomescreen();

    // Buttons
    pinMode(BUTTON_1, INPUT);
    pinMode(BUTTON_2, INPUT);
    pinMode(BUTTON_3, INPUT);

    pinMode(voltage, OUTPUT);
    digitalWrite(voltage, LOW);
    pinMode(hum_soil, INPUT);
    pinMode(sound, OUTPUT);

    // RFM69
    /*Serial.print("OpenCansat ");
      Serial.print(MYNODEID,DEC);
      Serial.println(" is ready");*/

    if (!radio.initialize(FREQUENCY, MYNODEID, NETWORKID))
    {
      isRadioOk = false;
      // Serial.println("RFM69HW initialization failed!");
    }
    else
    {
      radio.setFrequency(FREQUENCYSPECIFIC);
      radio.setHighPower(true); // Always use this for RFM69HW
    }
    //// RFM69
  }

//String bpej[3];

void loop()
  {   
    
    int button1 = digitalRead(BUTTON_1);
    int button2 = digitalRead(BUTTON_2);
    int button3 = digitalRead(BUTTON_3);

    /*while (Serial.available()) {
      String rev = Serial.readString();

      bpej[0] = getValue(rev,';',0);
      bpej[1] = getValue(rev,';',1);
      bpej[2] = getValue(rev,';',2);
    }*/

    if (button1 == LOW)
    {
      if (screenNum == 1)
      {
        screenNum = 0;
        ocsDesign.drawLeftScreen();
        delay(300);
        //	Serial.println("Button 1 ");
      }
      else if (screenNum == 2)
      {
        screenNum = 1;
        ocsDesign.drawHomescreen();
        delay(300);
        //Serial.println("Button 1 ");
      }
      /*else if (screenNum == 3)
      {
        screenNum = 2;
        ocsDesign.drawRightScreen();
        delay(300);
        //Serial.println("Button 1 ");
      }*/
    }
    else if (button2 == LOW)
    {
      if (screenNum != 1)
      {
        screenNum = 1;
        ocsDesign.drawHomescreen();
        delay(300);
        //Serial.println("Button 2 ");
      }
    }
    else if (button3 == LOW)
    {
      if (screenNum == 1)
      {
        screenNum = 2;
        ocsDesign.drawRightScreen();
        delay(300);
        //Serial.println("Button 3 ");
      }
      else if (screenNum == 0)
      {
        screenNum = 1;
        ocsDesign.drawHomescreen();
        delay(300);
        //Serial.println("Button 3 ");
      }
      /*else if (screenNum == 2){
        screenNum = 3;
        ocsDesign.drawBPEJscreen();
        delay(300);  
      }*/
    }

    if (radio.receiveDone()) // Got one!
    {
      //Serial.println("Message received: " + String(income.messageId));

      tone(sound, 2400);
      
      income = *(OcsStorage::message*)radio.DATA;   

      Serial.print("SATELLITE;");
      Serial.print(income.messageId);
      Serial.print(";");
      Serial.print(income.temperature, 2);
      Serial.print(";");
      Serial.print(income.humidity_bme280, 3);
      Serial.print(";");
      Serial.print(income.altitude, 3);
      Serial.print(";");
      Serial.print(income.pressure, 3);
      Serial.print(";");
      Serial.print(income.latitude, 6);
      Serial.print(";");
      Serial.print(income.longitude, 6);  
      Serial.println();
      Serial.flush();

      delay(200);

      noTone(sound);

      if (screenNum != 2){
      ocsData.Update(income, screenNum, null, null);
      }
    }

    if (millis() - time_last > 60000){
      
      digitalWrite(voltage, HIGH);
      delay(100);

      float data[6];
      data[0] = lightMeter.readLightLevel();
      data[1] = bme.readTemperature();
      data[2] = bme.readHumidity();
      data[3] = (bme.readPressure() / 100.0F);
      data[4] = bme.readAltitude(1013.25F);
      data[5] = analogRead(hum_soil);
      
      Serial.print("BASE-STATION;");
      Serial.print(data[0]);   
      Serial.print(";");
      Serial.print(data[1],2);
      Serial.print(";");
      Serial.print(data[2],2);
      Serial.print(";");
      Serial.print(data[3],2);
      Serial.print(";");
      Serial.print(data[4],2);
      Serial.print(";");
      Serial.print(data[5]);
      Serial.println();
      Serial.flush();

      digitalWrite(voltage, LOW);

      time_last = millis();

      if (screenNum == 2){
      ocsData.Update(income, screenNum, data, null);
      }

      delay(50);

    }

    delay(50);


}

/*String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}*/
