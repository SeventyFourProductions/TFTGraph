#include "TFTGraph.h"
#include "InternalUtils.h"
#include "math.h"

double InternalUtils::sind(double deg) {
    return sin(deg * M_PI / 180.0);
}

double InternalUtils::cosd(double deg) {
    return cos(deg * M_PI / 180.0);
}

void InternalUtils::drawDiagramBody(Adafruit_TFTLCD &gfx, int x, int y, uint16_t width, uint16_t height, float min, float max){

    bool isLightBackground = InternalUtils::isLightBackground(gfx, x, y);
    uint16_t drawColor = 0xFFFF;
    if(isLightBackground) drawColor = 0x0000;

    for (uint8_t i = 0; ; i++) {
        int yPos = y + i * 15;
        if (yPos > y + height) break;

        gfx.drawFastHLine(x, yPos, width, 0x8410);

        float t = (float)(yPos - y) / height;
        float value = max - t * (max - min);

	    if ((y + height) - (y+(i * 15)) < 10) break;

        String str = String(value, 1);
        gfx.setCursor((x - 3) - (6 * str.length()), yPos - 4);
        gfx.setTextColor(drawColor);
        gfx.setTextSize(1);
        gfx.print(value, 1);
    }

    gfx.drawFastHLine(x, y + height, width, 0x8410);
    gfx.setCursor(x - 3 - 6 * String(min,1).length(), y + height - 4);
    gfx.print(min, 1);

    //drawing bounding box:
    gfx.drawRect(x,y,width+2,height,drawColor);
}

void InternalUtils::drawHalfCircle(Adafruit_TFTLCD &gfx, int xc, int yc, uint8_t r, uint16_t color){

    int16_t x = 0;
    int16_t y = r;
    int16_t p = 1 - r;

    while (x <= y) {
        if(yc + y <= yc) gfx.drawPixel(xc + x, yc + y, color);
        if(yc + y <= yc) gfx.drawPixel(xc - x, yc + y, color);
        if(yc - y <= yc) gfx.drawPixel(xc + x, yc - y, color);
        if(yc - y <= yc) gfx.drawPixel(xc - x, yc - y, color);
        if(yc + x <= yc) gfx.drawPixel(xc + y, yc + x, color);
        if(yc + x <= yc) gfx.drawPixel(xc - y, yc + x, color);
        if(yc - x <= yc) gfx.drawPixel(xc + y, yc - x, color);
        if(yc - x <= yc) gfx.drawPixel(xc - y, yc - x, color);

        x++;
        if (p < 0) {
            //Midpoint is inside circle
            p = p + 2 * x + 1;
        } else {
            //Midpoint is outside circle
            y--;
            p = p + 2 * x - 2 * y + 1;
        }
    }
}

void InternalUtils::drawScatterBody(Adafruit_TFTLCD &gfx, int x, int y, uint16_t width, uint16_t height, float xMin, float xMax, float yMin, float yMax){

    bool isLightBackground = InternalUtils::isLightBackground(gfx, x, y);
    uint16_t drawColor = 0xFFFF;
    if(isLightBackground) drawColor = 0x0000;

    //Horizontal:
    for (uint8_t i = 0; ; i++) {
        int xPos = x + i * 30;
        if (xPos > x + width) break;

        gfx.drawFastVLine(xPos,y , height, 0x8410);

        float t = (float)(xPos - x) / width;
        float value = xMin + t * (xMax - xMin);

	    if ((x + width) - (x+(i * 30)) < 10) break;

        String str = String(value, 1);
        gfx.setCursor(xPos-4,y+height+10);
        gfx.setTextColor(drawColor);
        gfx.setTextSize(1);
        gfx.print(value, 1);
    }

    gfx.drawFastVLine(x+width, y,height, 0x8410);
    gfx.setCursor(x - 3 - 6 * String(yMin,1).length(), y + height - 4);
    gfx.print(yMin, 1);
  
    //Vertical:
    for (uint8_t i = 0; ; i++) {
        int yPos = y + i * 15;
        if (yPos > y + height) break;

        gfx.drawFastHLine(x, yPos, width, 0x8410);

        float t = (float)(yPos - y) / height;
        float value = yMax - t * (yMax - yMin);

	    if ((y + height) - (y+(i * 15)) < 10) break;

        String str = String(value, 1);
        gfx.setCursor((x - 3) - (6 * str.length()), yPos - 4);
        gfx.setTextColor(drawColor);
        gfx.setTextSize(1);
        gfx.print(value, 1);
    }

    gfx.drawFastHLine(x, y + height, width, 0x8410);
    gfx.setCursor(x - 3 - 6 * String(yMin,1).length(), y + height - 4);
    gfx.print(yMin, 1);
}

void InternalUtils::sortArray(float data[], int start, int end){
    //Insertion sort, O(n^2):
    for(int i = 1; i<(end-start); i++){
        float key = data[i];
        int j = i-1;

        while (j >= 0 && data[j] > key) {
            data[j + 1] = data[j];
            j -= 1;
        }

        data[j+1] = key;
    }
}

float InternalUtils::getMedian(float data[], int start, int end){
	int len = end - start;

	if(len % 2 == 1){
    	return data[start + len/2];
	}else{
    	int mid1 = start + len/2 - 1;
    	int mid2 = start + len/2;
    	return (data[mid1] + data[mid2]) / 2.0f;
	}
}

//This code doesn't seem to work? I don't want the user to have to pass in the background color so this is just gonna remain returning false for now. Maybe it has something to do with the "readPixel" function?
bool InternalUtils::isLightBackground(Adafruit_TFTLCD &gfx, int x, int y){
    uint16_t color = gfx.readPixel(x, y);

    //return color > 0x7FFF;
    return false;
}