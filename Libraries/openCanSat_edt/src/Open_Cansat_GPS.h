#ifndef _OPEN_CANSAT_GPS_H
#define _OPEN_CANSAT_GPS_H

#include <Arduino.h>
#include <WString.h>
#include <stdint.h>

class OpenCansatGPS
{
public:
/*--- CONSTRUCTOR ---*/
	/**
	 * Non parametric constructor
	 */
	OpenCansatGPS();

/*--- PUBLIC METHODS ---*/
	/**
	 * Begin of GPS i2c communication
	 */
    void begin();

	/**
	 * Switch on debug print
	 */
    void debugPrintOn(uint32_t speed);

    /*
     * Get methods for time, date and gps data
     */
    uint16_t getYear();
    uint8_t getMonth();
    uint8_t getDay();
    uint8_t getHour();
    uint8_t getMinute();
    uint8_t getSecond();
    uint8_t getNumberOfSatellites();
    double getLat();
    double getLon();
    double getAlt();
    double getSpeed();
    double getHdop();

	/**
	 *  Get date and time in string format
	 */
    String getDateTimeString();

    /**
     * Read UBlox device, reading is end when fix is seen,
     * or when timeout has been reached.
     */
    bool scan(uint32_t timeout = 10000);

private:
/*--- PRIVATE VARIABLES ---*/
    // Debug print value
    bool 		isDebugPrint;
    // Recognize of receiving/not receiving state
    bool	 		isReceiving;
    // Recognize if time is already received
    bool        isTimeReceived;
    // Recognize if lat and lon are seen
    bool        isSeenLatLon;
    // Recognize if Alt is seen
    bool        isSeenAlt;

    // OpenCansat i2c addr storage
    uint8_t 		i2cAddr;
    char*   		inputBuffer;
    size_t		inputBufferSize;

/*--- DATA STRUCTURES ---*/
    struct gpsTimeAndDate
    {
    		uint16_t year;
    		uint8_t mth;
    		uint8_t day;
    		uint8_t hrs;
    		uint8_t min;
    		uint8_t sec;
    };

    struct gpsData
    {
    		double lat;
    		double lon;
    		uint8_t numSat;
    	    double speed;
    	    double alt;
    	    double hdop;
    };

    // GPS date and time
    gpsTimeAndDate timeDate;

    // GPS data
    gpsData data;

/*--- PRIVATE METHODS ---*/
    /**
     * Debug logging function
     */
    void log(String message);

    /**
     * Reset of variables to 0
     */
    void resetValues();

    /**
     * Parse line best on start of line
     */
    bool parseLine(const char * line);

    /**
     * Read one NMEA frame
     */
    bool readLine(uint32_t timeout = 10000);

    /**
     * Begin transmission and read one byte
     */
    uint8_t read();

    /**
     *  Compute and verify checksum CRC for line
     *
     *  Line must start with '$' and end with '*'
     */
    bool computeCrc(const char * line);

    /**
     * Convert lat/long degree format to decimal degrees
     */
    double convertToDecimalDeg(const String & data);

    /**
     * Get field of specific index separator = ","
     */
    String getField(const String & data, int index);

    /**
     * Read the coordinates using GPGGA
     *
     * 2    lat     .. ddmm.mmmmm  .. Latitude (degrees & minutes)
     * 3    NS      .. char        .. North/South indicator
     * 4    long    .. dddmm.mmmmm .. Longitude (degrees & minutes)
     * 5    EW      .. char        .. East/West indicator
     * 6    quality .. digit       .. Quality indicator for position fix: 0 No Fix, 6 Estimated, 1 Auto GNSS, 2 Diff GNSS
     * 7    numSV   .. num         .. Number of satellites used
     * 8    hdop    .. num         .. Horizontal Dilution of Precision
     * 9    alt     .. num         .. Altitude above mean sea level
     */
    bool parseGPGGA(const String & line);

    /**
     * Read the coordinates using GPRMC
     *
     * 1    time    .. hhmmss.ss   .. UTC time
     * 2    status  .. char        .. Status, V = Navigation receiver warning, A = Data valid
     * 3    lat     .. ddmm.mmmmm  .. Latitude (degrees & minutes)
     * 4    NS      .. char        .. North/South
     * 5    long    .. dddmm.mmmmm .. Longitude (degrees & minutes)
     * 6    EW      .. char        .. East/West
     * 7    spd     .. num         .. Speed over ground
     * 9    date    .. ddmmyy      .. Date in day, month, year format
     * 12   posMode .. char        .. Mode Indicator: 'N' No Fix, 'E' Estimate, 'A' Auto GNSS, 'D' Diff GNSS
     */
    bool parseGPRMC(const String & line);

    /**
     * Read the coordinates using GNGLL
     *
     * 1    lat     .. ddmm.mmmmm  .. Latitude (degrees & minutes)
     * 2    NS      .. char        .. North/South
     * 3    long    .. dddmm.mmmmm .. Longitude (degrees & minutes)
     * 4    EW      .. char        .. East/West
     * 5    time    .. hhmmss.ss   .. UTC time
     * 6    status  .. char        .. Status, V = Navigation receiver warning, A = Data valid
     */
    bool parseGNGLL(const String & line);

    /*!
     * Parse GPGSV message
     *
     * Index 3 .. numSV .. Number of satellites in view
     */
    bool parseGPGSV(const String & line);
};

#endif // _OPEN_CANSAT_GPS_H
