
#ifndef _OCSGRAPHICS_HH
#define _OCSGRAPHICS_HH

#include <Arduino.h>
#include "Ucglib.h"

class OcsGraphics
{
  public:
	OcsGraphics(Ucglib_ST7735_18x128x160_HWSPI& ucgIn);

	void drawHomescreen();
	void drawRightScreen();
	void drawLeftScreen();
	void drawBPEJscreen();

	void drawBackground();

	void drawTemp(float temp);
	void drawMinTemp(float minTemp);
	void drawMaxTemp(float maxTemp);

	void drawPress(float pressure);
	void drawMinPress(float minPressure);
	void drawMaxPress(float maxPressure);

	void drawAltitude(float altitude);

	void drawCon(bool con);
	void drawConSideBars(bool con);

	void drawLightBS(float light);
	void drawTempBS(float temp);
	void drawHumBS(float hum);
	void drawPressBS(float press);
	void drawAltBS(float alt);
	void drawSoilBS(float soil);

	void drawBPEJ(String bpej);
	void drawDistrict(String district);
	void drawPlant(String plant);

	void drawMessageId(uint16_t messageId);
	void drawHumidity(float humidity);
	void drawLongitude(float longitude);
	void drawLatitude(float latitude);
	void drawDate(uint8_t day, uint8_t month, uint16_t year);
	void drawNumOfSat(uint8_t numOfSat);
	void drawPower(float power);

	void drawTime(int x, int y, String time);

	void setFontColor(uint8_t r, uint8_t g, uint8_t b);

  private:
	Ucglib_ST7735_18x128x160_HWSPI& ucg;

	uint8_t fontR = 255;
	uint8_t fontG = 255;
	uint8_t fontB = 255;

	uint8_t bgrR = 17;
	uint8_t bgrG = 30;
	uint8_t bgrB = 108;

	int divide(int x, double y);

	void drawHome(int x, int y, int width, int height, int r, int g, int b);

	void drawRightArrow(int x, int y);

	void drawLeftArrow(int x, int y);

	void drawFullBox(String text, int x, int y, int sizeX);

	void drawBox(int xFont, int yFont1, int start);

	void drawHumidityBox();

	void drawBackgroundStatic();

	void drawData(String data, int x, int y, const ucg_fntpgm_uint8_t *font, String unit, int fontSize);
};

#endif /* _OCSGRAPHICS_HH */

