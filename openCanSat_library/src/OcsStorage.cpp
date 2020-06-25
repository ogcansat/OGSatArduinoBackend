#include <OcsStorage.h>

OcsStorage::OcsStorage(OcsGraphics& ocsDesignIn)
 : isInitalize(false),
   isTransition(false),
   ocsDesign(ocsDesignIn)
{
}

float OcsStorage::getTemp()
{
	return this->actualData.temperature;
}

float OcsStorage::getMinTemp()
{
	return this->minData.temperature;
}

OcsStorage::message OcsStorage::getActualData()
{
	return this->actualData;
}

void OcsStorage::Update(message income, uint8_t screenNum, bool con)
{
	if(screenNum == 1)
	{
		if(this->isInitalize == false)
		{
			ocsDesign.drawTemp(income.temperature);
			ocsDesign.drawMinTemp(income.temperature);
			ocsDesign.drawMaxTemp(income.temperature);
			ocsDesign.drawPress(income.pressure);
			ocsDesign.drawMinPress(income.pressure);
			ocsDesign.drawMaxPress(income.pressure);
			ocsDesign.drawAltitude(income.altitude);
			ocsDesign.drawHumidity(income.humidity_bme280);
			minData.temperature = income.temperature;
			maxData.temperature = income.temperature;
			minData.pressure = income.pressure;
			maxData.pressure = income.pressure;
			isInitalize = true;
		}
		else if(this->isTransition == true)
		{
			ocsDesign.drawTemp(income.temperature);
			ocsDesign.drawMinTemp(minData.temperature);
			ocsDesign.drawMaxTemp(maxData.temperature);
			ocsDesign.drawPress(income.pressure);
			ocsDesign.drawMinPress(minData.pressure);
			ocsDesign.drawMaxPress(maxData.pressure);
			ocsDesign.drawAltitude(income.altitude);
			ocsDesign.drawHumidity(income.humidity_bme280);
			isTransition = false;
		}
		else
		{
			checkTemperature(income.temperature);
			checkPressure(income.pressure);
			checkAltitude(income.altitude);
			checkHumidity(income.humidity_bme280);
		}
		ocsDesign.drawMessageId(income.messageId);
		ocsDesign.drawConSideBars(con);
	}
	else if(screenNum == 0)
	{
		String hour;
		String minute;
		String sec;

		if(income.hour < 10)
		{
			hour += "0";
		}

		if(income.minute < 10)
		{
			minute += "0";
		}

		if(income.sec < 10)
		{
			sec += "0";
		}

		hour += String(income.hour);
		minute += String(income.minute);
		sec += String(income.sec);

		String time = hour + ":" + minute  + ":" + sec;

		ocsDesign.drawTime(13, 47, time);
		ocsDesign.drawDate(income.day, income.month, income.year);

		ocsDesign.drawLongitude(income.longitude);
		ocsDesign.drawLatitude(income.latitude);
		ocsDesign.drawNumOfSat(income.num_of_satelites);

		float loadvoltage = income.voltage_bus + (income.voltage_shunt / 1000);
		float power = income.current_mA * loadvoltage;

		ocsDesign.drawPower(power);
		
		ocsDesign.drawConSideBars(con);

		this->isTransition = true;

	}
	else if(screenNum == 2)
	{
		ocsDesign.drawCon(con);
		this->isTransition = true;
	}
	
}

// ToDo -10 value size is over the box
void OcsStorage::checkTemperature(float temp)
{
	if(temp > -40 and temp < 80)
	{
		if(actualData.temperature != temp)
		{
			this->actualData.temperature = temp;
			ocsDesign.drawTemp(temp);
		}

		if(temp < this->minData.temperature)
		{
			this->minData.temperature = temp;
			ocsDesign.drawMinTemp(temp);
		}

		if(temp > this->maxData.temperature)
		{
			this->maxData.temperature = temp;
			ocsDesign.drawMaxTemp(temp);
		}
	}
	else
	{
		// ToDo error handling
	}
}

void OcsStorage::checkPressure(float pressure)
{
	if(pressure > 30000 and pressure < 100000)
	{
		if(actualData.pressure != pressure)
		{
			this->actualData.pressure = pressure;
			ocsDesign.drawPress(pressure);
		}

		if(pressure < this->minData.pressure)
		{
			this->minData.pressure = pressure;
			ocsDesign.drawMinPress(pressure);
		}

		if(pressure > this->maxData.pressure)
		{
			this->maxData.pressure = pressure;
			ocsDesign.drawMaxPress(pressure);
		}
	}
	else
	{
		// ToDo error handling
	}
}

void OcsStorage::checkAltitude(float altitude)
{
	if(altitude > 0 and altitude < 5000)
	{
		if(this->actualData.altitude != altitude)
		{
			this->actualData.altitude = altitude;
			ocsDesign.drawAltitude(altitude);
		}
	}
	else
	{
		// ToDo error handling
	}
}

void OcsStorage::checkHumidity(float humidity)
{
	if(humidity > 0 and humidity < 100)
	{
		if(this->actualData.humidity_bme280 != humidity)
		{
			this->actualData.humidity_bme280 = humidity;
			ocsDesign.drawHumidity(humidity);
		}
	}
	else
	{
		// ToDo error handling
	}
}

