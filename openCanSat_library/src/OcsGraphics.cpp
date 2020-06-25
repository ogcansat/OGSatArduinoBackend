
#include "OcsGraphics.h"
#include "Ucglib.h"
#include "Time.h"

OcsGraphics::OcsGraphics(Ucglib_ST7735_18x128x160_HWSPI& ucgIn)
	:ucg(ucgIn)
{
}

void OcsGraphics::drawLeftScreen()
{
	ucg.setRotate90();

	ucg.setColor(17, 30, 108);

	// Fraw frame
	ucg.drawBox(2, 10, 156, 103);
	ucg.drawBox(0, 110, 40, 18);
	ucg.drawBox(140, 110, 40, 18);

	drawFullBox("LON", 81, 22, 35);
	drawFullBox("LAT", 118, 22, 35);

	// NUM OF SATS
	ucg.setColor(255, 255, 255);

	ucg.setPrintPos(85, 55);

	ucg.setFont(ucg_font_micro_mf);

	ucg.print("NUM OF SATELITES:");
	// END NUM OF SATS

	// INA219
	ucg.setPrintPos(90, 86);

	ucg.setFont(ucg_font_micro_mf);

	ucg.print("POWER:");

	ucg.drawRFrame(83, 76, 74, 30, 3);
	// INA219

	ucg.setColor(255, 255, 255);

	ucg.drawRBox(5, 22, 74, 40, 3);

  	this->drawRightArrow(141, 118);
}

void OcsGraphics::drawRightScreen()
{
	ucg.setRotate90();

	ucg.setColor(17, 30, 108);

	ucg.setFont(ucg_font_inr16_mr);

	ucg.drawBox(2, 10, 156, 103);
	//ucg.drawBox(140, 110, 40, 18);

	ucg.setColor(255, 255, 255);

	ucg.setPrintPos(10, 45);
	// Text to print
	ucg.print("Conditions:");	

  	this->drawLeftArrow(10, 118);
}

void OcsGraphics::drawHomescreen()
{
  ucg.setRotate90();

  ucg.setColor(17, 30, 108);

  // Draw frame
  ucg.drawBox(2, 10, 156, 103);

  drawFullBox("MESSAGE ID", 5, 22, 74);

  drawFullBox("ALTITUDE", 81, 22, 74);

  drawBox(8, 68, 5);

  drawBox(84, 68, 81);

  drawHumidityBox();

  ucg.setColor(255, 255, 255);

  ucg.drawRFrame(5, 88, 150, 20, 3);

  this->drawRightArrow(141, 118);
  this->drawLeftArrow(10, 118);
}

void OcsGraphics::drawBackground()
{
  drawBackgroundStatic();
}

int OcsGraphics::divide(int x, double y)
{
	return ceil(x/y);
}

void OcsGraphics::drawCon(bool con)
{
	ucg.setColor(255, 255, 255);
	ucg.drawBox(0, 50, 200, 30);
	// Text to print, 67 48

	if(con){
	ucg.setColor(5, 154, 0);
	ucg.setPrintPos(50, 72);
	ucg.print("Good");
	}
	else{
	ucg.setColor(255, 0, 0);
	ucg.setPrintPos(35, 72);
	ucg.print("Bad/NEI");
	}	
}

void OcsGraphics::drawConSideBars(bool con)
{
	if(con){
	ucg.setColor(5, 154, 0);
	ucg.drawBox(48, 155, 30, 5);
	ucg.drawBox(48, 0, 30, 5);
	}
	else{
	ucg.setColor(255, 0, 0);
	ucg.drawBox(48, 155, 30, 5);
	ucg.drawBox(48, 0, 30, 5);
	}
}

void OcsGraphics::drawHome(int x, int y, int width, int height, int r, int g, int b)
{
	int halfSizeHeight = divide(height, 2);
	int halfSizeWidth = divide(width, 2);

	ucg.setColor(255, 255, 255);

	ucg.drawBox(x + divide(width, 4), y + halfSizeHeight, halfSizeWidth, halfSizeHeight);

	ucg.drawTriangle(x + halfSizeWidth, y, x + width, y + halfSizeHeight, x, y + halfSizeHeight);

	ucg.setColor(r, g, b);

	ucg.drawBox(x + 5*divide(width, 12), y + 4*divide(height, 6), divide(width, 6), divide(height, 3));

	ucg.setColor(255, 255, 255);

	ucg.drawRFrame(5, 88, 150, 20, 3);
}

void OcsGraphics::drawRightArrow(int x, int y)
{
  ucg.setColor(255, 255, 255);

  ucg.drawBox(x, y, 8, 5);

  ucg.drawTriangle(x + 8, y - 4, x + 8, y + 8, x + 14, y + 2);
}

void OcsGraphics::drawLeftArrow(int x, int y)
{
  ucg.setColor(255, 255, 255);

  ucg.drawBox(x, y, 8, 5);

  ucg.drawTriangle(x, y - 5, x, y + 8, x - 6, y + 2);
}

void OcsGraphics::drawBackgroundStatic()
{
  ucg.setRotate90();

  // Set position
  ucg.setPrintPos(0, 0);

  // Set color
  ucg.setColor(17, 30, 108);

  // draw rectangle (background)
  ucg.drawBox(0, 0, 160, 128);

  // Set color
  ucg.setColor(255, 255, 255);

  // Fraw frame
  ucg.drawRFrame(2, 10, 156, 103, 5);

  // Set text position
  ucg.setPrintPos(90, 7);

  ucg.setFontPosBaseline();

  // set font
  ucg.setFont(ucg_font_5x7_mf);

  // Text to print
  ucg.print("openCansat.eu");

  this->drawRightArrow(141, 118);

  this->drawLeftArrow(10, 118);

  this->drawHome(74, 114, 12, 11, 17, 30, 108);

  ucg.setRotate270();
}

void OcsGraphics::setFontColor(uint8_t r, uint8_t g, uint8_t b)
{
	this->fontR = r;
	this->fontG = g;
	this->fontB = b;
}

void OcsGraphics::drawTemp(float temp)
{
	drawData(String(temp, 1), 16, 58, ucg_font_7x13B_tf, " C", 7);
}

void OcsGraphics::drawMinTemp(float minTemp)
{
	drawData(String(minTemp, 1), 30, 68, ucg_font_5x7_mf, " C", 5);
}

void OcsGraphics::drawMaxTemp(float maxTemp)
{
	drawData(String(maxTemp, 1), 30, 79, ucg_font_5x7_mf, " C", 5);
}

void OcsGraphics::drawPress(float pressure)
{
	drawData(String(pressure/100, 1), 90, 58, ucg_font_7x13B_tf, " hPa", 7);
}

void OcsGraphics::drawMinPress(float minPressure)
{
	drawData(String(minPressure/100, 1), 106, 68, ucg_font_5x7_mf, " hPa", 5);
}

void OcsGraphics::drawMaxPress(float maxPressure)
{
	drawData(String(maxPressure/100, 1), 106, 79, ucg_font_5x7_mf, " hPa", 5);
}

void OcsGraphics::drawPower(float power)
{
	drawData(String(power), 90, 100, ucg_font_7x13B_tf, " mW", 5);
}

void OcsGraphics::drawNumOfSat(uint8_t numOfSat)
{
	drawData(String(numOfSat), 113, 69, ucg_font_7x13B_tf, "", 5);
}

void OcsGraphics::drawLongitude(float longitude)
{
	this->bgrB = 255;
	this->bgrG = 255;
	this->bgrR = 255;
	this->fontR = 17;
	this->fontG = 30;
	this->fontB = 108;

	drawData(String(longitude, 1), 94, 36, ucg_font_7x13B_tf, "", 7);

	this->bgrR = 17;
	this->bgrG = 30;
	this->bgrB = 108;
	this->fontB = 255;
	this->fontG = 255;
	this->fontR = 255;
}

void OcsGraphics::drawLatitude(float latitude)
{
	this->bgrB = 255;
	this->bgrG = 255;
	this->bgrR = 255;
	this->fontR = 17;
	this->fontG = 30;
	this->fontB = 108;

	drawData(String(latitude, 1), 131, 36, ucg_font_7x13B_tf, "", 7);

	this->bgrR = 17;
	this->bgrG = 30;
	this->bgrB = 108;
	this->fontB = 255;
	this->fontG = 255;
	this->fontR = 255;
}

void OcsGraphics::drawAltitude(float altitude)
{
	this->bgrB = 255;
	this->bgrG = 255;
	this->bgrR = 255;
	this->fontR = 17;
	this->fontG = 30;
	this->fontB = 108;

	drawData(String(altitude, 1), 90, 36, ucg_font_7x13B_tf, " m", 7);

	this->bgrR = 17;
	this->bgrG = 30;
	this->bgrB = 108;
	this->fontB = 255;
	this->fontG = 255;
	this->fontR = 255;
}

void OcsGraphics::drawMessageId(uint16_t messageId)
{
	this->bgrB = 255;
	this->bgrG = 255;
	this->bgrR = 255;
	this->fontR = 17;
	this->fontG = 30;
	this->fontB = 108;

	drawData(String(messageId), 16, 36, ucg_font_7x13B_tf, "", 7);

	this->bgrR = 17;
	this->bgrG = 30;
	this->bgrB = 108;
	this->fontB = 255;
	this->fontG = 255;
	this->fontR = 255;
}

void OcsGraphics::drawData(String data, int x, int y, const ucg_fntpgm_uint8_t *font, String unit, int fontSize)
	{
	  data += unit;

	  ucg.begin(UCG_FONT_MODE_SOLID);
	  ucg.setRotate90();
	  ucg.setColor(fontR, fontG, fontB);
	  ucg.setColor(1, bgrR, bgrG, bgrB);

	  ucg.setPrintPos(x, y);

	  ucg.setFont(font);

	  ucg.print(data);

	  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
	}

void OcsGraphics::drawHumidity(float humidity)
{
	 ucg.setRotate90();

	 ucg.setFont(ucg_font_6x12_67_75);

	 const int xStart = 58;

	 int rgColor = 255;
	 int xMoveCount = 0;
	 int colorDownCount = 0;

	 for(int i = 5; i < 100; i += 15)
	 {
		 if(humidity > i)
		 {
			 ucg.setColor(rgColor, rgColor, 204 - colorDownCount);

			 if(colorDownCount<= 204)
			 {
				 colorDownCount += 50;
			 }else
			 {
				 rgColor -= 30;
			 }
		 }
		 else
		 {
			 ucg.setColor(this->bgrR, this->bgrG, this->bgrB);
		 }

		 ucg.drawGlyph(xStart + xMoveCount, 109, 0, 128);
		 xMoveCount += 12;
	 }
}

void OcsGraphics::drawHumidityBox()
{
  ucg.setColor(255, 255, 255);

  ucg.setPrintPos(17, 101);

  ucg.setFontPosBaseline();

    // set font
  ucg.setFont(ucg_font_4x6_mf);

  // Text to print
  ucg.print("HUMIDITY:");

  // NUMBERS

  ucg.setPrintPos(57, 97);

    // set font
  ucg.setFont(ucg_font_4x6_tf);

  // Text to print
  ucg.print("05 20 35 50 65 80 95 %");

}

// "ALTITUDE, 82, 22, 67"
void OcsGraphics::drawFullBox(String text, int x, int y, int sizeX)
{
  ucg.setColor(255, 255, 255);

  ucg.setPrintPos(x + 2, y - 2);

  ucg.setFont(ucg_font_5x7_mf);

  ucg.print(text);

  ucg.setColor(255, 255, 255);

  ucg.drawRBox(x, y, sizeX, 20, 3);
}

void OcsGraphics::drawDate(uint8_t day, uint8_t month, uint16_t year)
{

	ucg.setColor(fontR, fontG, fontB);
	ucg.setColor(1, bgrR, bgrG, bgrB);

	ucg.setRotate90();

	String sDay;
	String sMonth;

	if(day<10)
	{
		sDay += "0";
	}

	switch(month)
	{
		case 0:
		case 1:
			sMonth = " January";
			break;
		case 2:
			sMonth = " February";
			break;
		case 3:
			sMonth = "March";
			break;
		case 4:
			sMonth = "April";
			break;
		case 5:
			sMonth = "May";
			break;
		case 6:
			sMonth = "June";
			break;
		case 7:
			sMonth = "July";
			break;
		case 8:
			sMonth = "August";
			break;
		case 9:
			sMonth = "September";
			break;
		case 10:
			sMonth = "October";
			break;
		case 11:
			sMonth = "November";
			break;
		case 12:
			sMonth = "December";
			break;
	}


	if(day==0 or year==0)
	{
		sDay += 1;
		year = 1990;
	}
	ucg.setPrintPos(7, 78);
	ucg.setFont(ucg_font_6x13_tf);
	ucg.print(sDay + sMonth);

	ucg.setPrintPos(3, 107);
	ucg.setFont(ucg_font_courB24_mn);
	ucg.print(String(year));
}

void OcsGraphics::drawTime(int x, int y, String time)
{
  ucg.begin(UCG_FONT_MODE_SOLID);

  ucg.setRotate90();

  ucg.setColor(bgrR, bgrG, bgrB);
  ucg.setColor(1, fontR, fontR, fontB);
  ucg.setPrintPos(x, y);
  ucg.setFont(ucg_font_helvB10_tr);

  ucg.print(time);

  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
}


void OcsGraphics::drawBox(int xFont, int yFont1, int start)
{
    // set font
  ucg.setFont(ucg_font_5x7_tf);

  ucg.setColor(80, 255, 80);

  ucg.setPrintPos(xFont, yFont1);

  // Text to print
  ucg.print("MIN:");

  ucg.setPrintPos(xFont, yFont1 + 11);

  ucg.setColor(255, 80, 80);

  // Text to print
  ucg.print("MAX:");

  ucg.setColor(255, 255, 255);

  ucg.drawRFrame(start, 45, 74, 40, 3);
}


