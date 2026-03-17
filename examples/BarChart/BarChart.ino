#include <Adafruit_TFTLCD.h>
#include <TFTGraph.h>

#define BLACK 0x0000
#define WHITE 0xFFFF

Adafruit_TFTLCD tft;
TFTGraph gfx(tft);

void setup() {
  tft.begin(0x9325);
  tft.reset();
  tft.setRotation(1);

  tft.fillScreen(BLACK);

  //synthetic game rating distribution dataset:
  float data[] = {
    0.5,  // 0
    1.2,  // 1
    2.4,  // 2
    4.8,  // 3
    8.5,  // 4
    14.0, // 5
    21.5, // 6
    23.0, // 7
    16.2, // 8
    7.1,  // 9
    0.8   // 10
  };

  const int x = 40;
  const int y = 30;
  const uint16_t width = 270;
  const uint16_t height = 190;
  const int start = 0;
  const int end = sizeof(data)/sizeof(data[0]);
  const uint16_t color = 0xFF45;
  const bool drawBackground = true;

  tft.setCursor(x, y-11);
  tft.setTextColor(WHITE);
  tft.print("Game rating distribution:");

  /*
  input:
  int x, int y, uint16_t width, uint16_t height, float data[], int start, int end, uint16_t color, bool drawBackground
  */

  gfx.drawBarChart(x, y, width, height, data, start, end, color, drawBackground);
}

void loop() {
}