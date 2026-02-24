#ifndef TFTUTILS_H
#define TFTUTILS_H

#include "Arduino.h"

class InternalUtils {
    public:
        static double sind(double deg);
        static double cosd(double deg);

        static void drawDiagramBody(Adafruit_TFTLCD &gfx, int x, int y, uint16_t width, uint16_t height, float min, float max);

        static void drawHalfCircle(Adafruit_TFTLCD &gfx, int xc, int yc, uint8_t r, uint16_t color);

        static void drawScatterBody(Adafruit_TFTLCD &gfx, int x, int y, uint16_t width, uint16_t height, float xMin, float xMax, float yMin, float yMax);

        static void sortArray(float data[], int start, int end);

        static float getMedian(float data[], int start, int end);
};

#endif