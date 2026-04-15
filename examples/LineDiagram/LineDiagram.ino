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


  //Approximation of average monthly temperature in °C (2025), Oslo. From January to December:
  float data[] {
  -3.2,
  -2.7,
  0.4,
  5.6,
  10.3,
  15.2,
  18.4,
  17.6,
  11.3,
  7.2,
  1.1,
  -2.8
  };

  const int x = 30;
  const int y = 75;
  const uint16_t width = 225;
  const uint16_t height = 125;
  const uint16_t start = 0;
  const uint16_t end = sizeof(data)/sizeof(data[0]);
  const float min = -5.0;
  const float max = 20.0;
  const uint8_t lineThickness = 2;
  const uint16_t color = gfx.getRGB565FromRGB888(0, 255, 0);
  const bool drawBackground = true;
  const bool fill = false;

  tft.setCursor(x, y-11);
  tft.setTextColor(WHITE);
  tft.print("Average monthly temperature in celsius, Oslo");

  /*
  Input:
  int x, int y, uint16_t width, uint16_t height, float data[], uint16_t start, uint16_t end, float min, float max, uint8_t lineThickness, uint16_t color, bool drawBackground, bool fill
  */
  gfx.drawLineDiagram(x, y, width, height, data, start, end, min, max, lineThickness, color, drawBackground, fill);

  /*
  If you wish, you could very well add your own labels at the bottom, for every month.
  All you need to know is that the y-position of the bottom of the diagram is (y+height).
  */
}

void loop() {
}