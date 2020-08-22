#include "Open_Cansat_GPS.h"

#include <Arduino.h>
#include <Wire.h>

/*--- CONFIG ---*/
// I2C address set
static const uint8_t openCansat_gps_i2c_addr = 0x42;
static const size_t init_ublox_buffer_size = 128;

/**
 * Check if timeout occured
 */
static inline bool isTimeOut(uint32_t from, uint32_t timeout)
{
    return (millis() - from) > timeout;
}

OpenCansatGPS::OpenCansatGPS()
	: isDebugPrint(false),
	  isReceiving(false),
	  i2cAddr(openCansat_gps_i2c_addr),
	  inputBuffer(static_cast<char*>(malloc(init_ublox_buffer_size))),
	  inputBufferSize(init_ublox_buffer_size)
{
    resetValues();
}

void OpenCansatGPS::begin()
{
    Wire.begin();
}

void OpenCansatGPS::debugPrintOn(uint32_t speed)
{
	this->isDebugPrint = true;

	SerialUSB.begin(speed);
}

uint16_t OpenCansatGPS::getYear()
{
	return (uint16_t)timeDate.year + 2000;
}

uint8_t OpenCansatGPS::getMonth()
{
	return timeDate.mth;
}

uint8_t OpenCansatGPS::getDay()
{
	return timeDate.day;
}

uint8_t OpenCansatGPS::getHour()
{
	return timeDate.hrs;
}

uint8_t OpenCansatGPS::getMinute()
{
	return timeDate.min;
}

uint8_t OpenCansatGPS::getSecond()
{
	return timeDate.sec;
}

uint8_t OpenCansatGPS::getNumberOfSatellites()
{
	return data.numSat;
}

double OpenCansatGPS::getLat()
{
	return data.lat;
}

double OpenCansatGPS::getLon()
{
	return data.lon;
}

double OpenCansatGPS::getAlt()
{
	return data.alt;
}

double OpenCansatGPS::getSpeed()
{
	return data.speed;
}

double OpenCansatGPS::getHdop()
{
	return data.hdop;
}

String OpenCansatGPS::getDateTimeString()
{
    return String(getYear()) + " " + String(getMonth()) + " " + String(getDay())
           + " " +  String(getHour()) + " " +  String(getMinute()) + " " +  String(getSecond());
}

bool OpenCansatGPS::scan(uint32_t timeout)
{
    bool result = false;

    uint32_t start = millis();

    resetValues();

    while (!isTimeOut(start, timeout))
    {
        if (!readLine()) {
            continue;
        }

        parseLine(inputBuffer);

        if (isSeenLatLon && data.numSat > 0)
        {
            result = true;
            break;
        }
    }

    if (data.numSat > 0)
    {
    		log(String("[scan] num sats = ") + data.numSat);
    }

    if (isSeenLatLon)
    {
    	    log(String("[scan] lat = ") + String(data.lat, 7));
    	    log(String("[scan] lon = ") + String(data.lon, 7));
    }

    return result;
}

void OpenCansatGPS::log(String message)
{
	if(isDebugPrint)
	{
		SerialUSB.println(message);
	}
}

void OpenCansatGPS::resetValues()
{
	timeDate = {0, 0, 0, 0, 0, 0};
	data = {0, 0, 0, 0, 0, 0};

    isSeenLatLon = false;
    isSeenAlt = false;
    isTimeReceived = false;
}

bool OpenCansatGPS::parseLine(const char * line)
{
    if (!computeCrc(line))
    {
        return false;
    }

    String data = line + 1;

    data.remove(data.length() - 3, 3);  // Strip checksum *<hex><hex>

    if (data.startsWith("GPGGA") || data.startsWith("GNGGA"))
    {
        return parseGPGGA(data);
    }

    if (data.startsWith("GPRMC") || data.startsWith("GNRMC"))
    {
        return parseGPRMC(data);
    }

    if (data.startsWith("GPGSV") || data.startsWith("GLGSV"))
    {
        return parseGPGSV(data);
    }

    if (data.startsWith("GNGLL"))
    {
    		return parseGNGLL(data);
    }

    log(String("No parse: ") + line);

    return false;
}

bool OpenCansatGPS::readLine(uint32_t timeout)
{
    if (!inputBuffer)
    {
        return false;
    }

    uint32_t startTime = millis();
    size_t counter = 0;
    char c = 0;

    char *ptr = inputBuffer;
    *ptr = '\0';

    while (!isTimeOut(startTime, timeout))
    {
        c = (char)read();
        if (c == '$')
        {
            break;
        }
    }

    if (c != '$')
    {
        return false;
    }

    *ptr++ = c;
    ++counter;

    c = 0;
    while (!isTimeOut(startTime, timeout))
    {
        c = (char)read();
        if (c == '\r')
        {
            continue;
        }
        if (c == '\n')
        {
            break;
        }
        if (counter < inputBufferSize - 1)
        {
            *ptr++ = c;
            ++counter;
        }
    }

    *ptr = '\0';

    if (c != '\n')
    {
        return false;
    }

    // End transmission
    if (isReceiving)
    {
        Wire.endTransmission();
        isReceiving = false;
    }

    return true;
}

uint8_t OpenCansatGPS::read()
{
	// Begin transmission
	if (!isReceiving)
	{
		Wire.beginTransmission(i2cAddr);
	    	isReceiving = true;
	}

	// Read byte by byte
    uint8_t b = 0xFF;
    uint8_t nr_bytes = Wire.requestFrom(i2cAddr, 1, false);
    if (nr_bytes == 1)
    {
        b = Wire.read();
    }
    return b;
}

bool OpenCansatGPS::computeCrc(const char* line)
{
    size_t len = strlen(line);
    String crc1String = "";
    uint8_t crc = 0;

    // Compute crc from line
    if (len < 4 or line[0] != '$' or line[len - 3] != '*')
    {
        	log("Invalid lengt or Begin/End character [$/*]");
        return false;
    }

    for (size_t i = 1; i < len - 3; ++i)
    {
        crc ^= line[i];
    }

    // Get crc1 from line (before end char)
    crc1String.operator +=(line[len - 2]);
    crc1String.operator +=(line[len - 1]);
    uint8_t crc1 = (int)strtol(crc1String.c_str(), 0, 16);

    // Compare both crc, they have to be equal
    if (crc != crc1)
    {
    	    log("Invalid CRC: " + String(crc1) +
    	    	    ", expected: " + String(crc));
        return false;
    }

    return true;
}

double OpenCansatGPS::convertToDecimalDeg(const String & data)
{
    double degMin = static_cast<int>(data.toFloat() / 100);

    double min = fmod(static_cast<double>(degMin), 100.0);

    return degMin + (min / 60);
}

String OpenCansatGPS::getField(const String & data, int index)
{
	int beginIndex = 0;
	int maxIndex = 0;

	for(int i = 0; i < index; i++)
	{
		beginIndex = data.indexOf(",", beginIndex+1);
	}

	maxIndex = data.indexOf(",", beginIndex+1);

	if(maxIndex == -1)
	{
		maxIndex = data.length() - 1;
	}

	return beginIndex == -1 ? "" : data.substring(beginIndex + 1, maxIndex);
}

bool OpenCansatGPS::parseGPGGA(const String & line)
{
	log(String("Parse: ") + line);

	data.lat 		= convertToDecimalDeg(getField(line, 2));
	String directNS 	= getField(line, 3);
	data.lon 		= convertToDecimalDeg(getField(line, 4));
	String directEW 	= getField(line, 5);
	String quality 	= getField(line, 6);
	data.numSat   	= getField(line, 7).toInt();
	data.hdop 		= getField(line, 8).toFloat();
	String alt    	= getField(line, 9);
	String altState 	= getField(line, 10);

    if (quality.compareTo("0") != 0)
    {
        if (directNS.compareTo("S") == 0)
        {
        		data.lat = -data.lat;
        }

        if (directEW.compareTo("W") == 0)
        {
        		data.lon = -data.lon;
        }

        isSeenLatLon = true;

        if (altState.compareTo("M") == 0)
        {
            data.alt = alt.toFloat();
            isSeenAlt = true;
        }
    }
    return true;
}

bool OpenCansatGPS::parseGNGLL(const String & line)
{
	log(String("Parse: ") + line);

	data.lat 		= convertToDecimalDeg(getField(line, 1));
    String directNS 	= getField(line, 2);
    data.lon 		= convertToDecimalDeg(getField(line, 3));
    String directEW 	= getField(line, 4);
    String time  	= getField(line, 5);
    String status 	= getField(line, 6);

    log(String("[scan] status = ") + String(status));

    if (status.compareTo("A") == 0)
    {
        if (directNS.compareTo("S") == 0)
        {
        		data.lat = -data.lat;
        }

        if (directEW.compareTo("W") == 0)
        {
        		data.lon = -data.lon;
        }

        isSeenLatLon = true;

        log(String("[scan] dataLon = ") + String(data.lon));
        log(String("[scan] dataLat = ") + String(data.lat));
    }

    if (time.length() == 9)
    {
    		timeDate.hrs = time.substring(0, 2).toInt() + 2;
    		timeDate.min = time.substring(2, 4).toInt();
    		timeDate.sec = time.substring(4, 6).toInt();

    		log(String("[scan] datetime = ") + String(timeDate.hrs)
    				+ " " + String(timeDate.min) + " " + String(timeDate.sec));
    }

    return true;
}

bool OpenCansatGPS::parseGPRMC(const String & line)
{
	log(String("Parse: ") + line);

    String time  	= getField(line, 1);
    String status 	= getField(line, 2);
    data.lat 		= convertToDecimalDeg(getField(line, 3));
    String directNS 	= getField(line, 4);
    data.lon 		= convertToDecimalDeg(getField(line, 5));
    String directEW 	= getField(line, 6);
    data.speed 		= getField(line, 7).toDouble();
    String date  	= getField(line, 9);
    String posMode 	= getField(line, 12);

    if (status.compareTo("A") == 0 && posMode.compareTo("N") != 0)
    {
        if (directNS.compareTo("S") == 0)
        {
        		data.lat = -data.lat;
        }

        if (directEW.compareTo("W") == 0)
        {
        		data.lon = -data.lon;
        }

        isSeenLatLon = true;
    }

    if (time.length() == 9 && date.length() == 6)
    {
		timeDate.day  = date.substring(0, 2).toInt();
		timeDate.mth  = date.substring(2, 4).toInt();
		timeDate.year = date.substring(4, 6).toInt();

    		timeDate.hrs = time.substring(0, 2).toInt() + 2;
    		timeDate.min = time.substring(2, 4).toInt();
    		timeDate.sec = time.substring(4, 6).toInt();

    		log(String("[scan] datetime = ") + getDateTimeString());
    }

    return true;
}

bool OpenCansatGPS::parseGPGSV(const String & line)
{
	log(String("Parse: ") + line);

    data.numSat = getField(line, 3).toInt();

    log(String("[scan] num sats = ") + data.numSat);

    return true;
}
