#include <Adafruit_TFTLCD.h>
#include <TFTGraph.h>

#define WHITE 0xFFFF
#define BLACK 0x0000

Adafruit_TFTLCD tft;
TFTGraph gfx(tft);

void setup() {
  tft.begin(0x9325);
  tft.reset();
  tft.setRotation(1);

  tft.fillScreen(BLACK);
  /*
  Array is sorted like this:
  1. x
  2. y
  3. color/group
  */

  //Scatter plot dataset with absolutely no meaning:
  const float data[][3] = {
    // Group 0 (lower-left-ish, spreads upward)
    {1.1, 2.3, 0},
    {1.4, 2.7, 0},
    {1.8, 3.0, 0},
    {2.0, 3.4, 0},
    {1.6, 3.1, 0},
    {2.2, 3.6, 0},
    {1.9, 2.9, 0},
    {2.4, 3.8, 0},
    {1.3, 2.5, 0},
    {2.1, 3.2, 0},
    {2.6, 3.9, 0},
    {1.7, 2.8, 0},
    {2.3, 3.5, 0},
    {1.5, 2.6, 0},
    {2.0, 3.1, 0},

    // Group 1 (overlaps with group 0 & 2)
    {2.8, 2.4, 1},
    {3.1, 2.7, 1},
    {3.4, 3.0, 1},
    {3.0, 2.9, 1},
    {3.6, 3.2, 1},
    {3.2, 2.6, 1},
    {3.8, 3.5, 1},
    {2.9, 2.5, 1},
    {3.5, 3.1, 1},
    {3.3, 2.8, 1},
    {4.0, 3.6, 1},
    {3.7, 3.3, 1},
    {3.1, 2.9, 1},
    {3.9, 3.4, 1},
    {3.4, 3.0, 1},

    // Group 2 (upper-middle, overlaps with group 1)
    {3.2, 3.8, 2},
    {3.6, 4.1, 2},
    {3.9, 4.4, 2},
    {4.1, 4.0, 2},
    {3.7, 4.2, 2},
    {4.4, 4.6, 2},
    {3.5, 3.9, 2},
    {4.0, 4.3, 2},
    {4.2, 4.1, 2},
    {3.8, 4.0, 2},
    {4.6, 4.7, 2},
    {3.9, 4.2, 2},
    {4.3, 4.5, 2},
    {3.6, 3.8, 2},
    {4.1, 4.4, 2},

    // Group 3 (right side, overlaps with group 2)
    {4.8, 3.6, 3},
    {5.1, 3.9, 3},
    {5.4, 4.2, 3},
    {5.0, 3.8, 3},
    {5.6, 4.5, 3},
    {5.2, 4.0, 3},
    {5.8, 4.6, 3},
    {4.9, 3.7, 3},
    {5.5, 4.3, 3},
    {5.3, 4.1, 3},
    {6.0, 4.8, 3},
    {5.7, 4.4, 3},
    {5.1, 3.9, 3},
    {5.9, 4.7, 3},
    {5.4, 4.2, 3}
};
//Groups are arbitrary, but need to be integers to properly work. The groups will get their assigned color based on the "colors" array.

  const uint16_t colors[] = {0x06ff, 0xf7e0, 0xf800, 0xf81d};

  const int x = 50;
  const int y = 30;
  const uint16_t width = 200;
  const uint16_t height = 150;
  const int start = 0;
  const int end = sizeof(data)/sizeof(data[0]);
  const bool drawBackground = true;

  tft.setCursor(x, y-11);
  tft.setTextColor(WHITE);
  tft.print("Scatter plot example:");
  /*
  input:
  int x, int y, uint16_t width, uint16_t height, float data[][3], int start, int end, uint16_t colors[], bool drawBackground
  */
  gfx.drawScatterPlot(x, y, width, height, data, start, end, colors, drawBackground);
}

void loop() {
}