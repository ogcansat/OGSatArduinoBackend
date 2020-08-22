#ifndef _OCSDATASTORAGE_HH
#define _OCSDATASTORAGE_HH

#include <Arduino.h>
#include "OcsGraphics.h"

class OcsStorage
{
  public:
	OcsStorage(OcsGraphics& ocsDesignIn);

	// define our own struct data type with variables
	typedef struct
	{
	  uint16_t messageId;
	  uint16_t year;
	  uint8_t month;
	  uint8_t day;
	  uint8_t hour;
	  uint8_t minute;
	  uint8_t sec;
	  float longitude;
	  float latitude;
	  uint8_t num_of_satelites;
	  float temperature;
	  float pressure;
	  float altitude;
	  float humidity_bme280;
	  float voltage_shunt;
	  float voltage_bus;
	  float current_mA;
	  float voltage_load;
	  int16_t rssi;
	} message;

	void Update(message income, uint8_t screenNum, float data[], String bpej[]);

	float getTemp();

	message getActualData();

	float getMinTemp();

  private:
	bool isInitalize;
	bool isTransition;

	OcsGraphics ocsDesign;

	message actualData {};
	message minData {};
	message maxData {};

	void checkTemperature(float temp);
	void checkPressure(float pressure);
	void checkAltitude(float altitude);
	void checkHumidity(float humidity);
};

#endif /* _OCSGRAPHICS_HH */

