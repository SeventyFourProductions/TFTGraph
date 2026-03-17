#include <Adafruit_GFX.h>
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

  //Synthetic dataset that slowly increases, has a drop off and then reaches the top:
  float data[] {
  0,
  1,
  3,
  6,
  10,
  15.5,
  4,
  7,
  9,
  20,
  24,
  26
  };

  const int x = 30;
  const int y = 75;
  const uint16_t width = 225;
  const uint16_t height = 125;
  const uint16_t start = 0;
  const uint16_t end = sizeof(data)/sizeof(data[0]);
  const float min = -1;
  const float max = 26;
  const uint8_t lineThickness = 1;
  const uint16_t color = 0x07e0; //0x07e0 is a bright green color.
  const uint16_t color2 = 0x07fe; //0x07fe is cyan.

  tft.setCursor(x, y-11);
  tft.setTextColor(WHITE);
  tft.print("Random dataset:");

  //the first diagram draws the background, and every subsequent diagram on top needs to have the background disabled:
  gfx.drawLineDiagram(x, y, width, height, data, start, end, min, max, lineThickness, color, true);
  gfx.drawPointDiagram(x, y, width, height, data, start, end, min, max, color2, false);
}

void loop() {
}