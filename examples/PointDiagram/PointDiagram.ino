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

  //Daily rainfall in mm over 12 days:
  float data[] {
    0.0,
    2.4,
    0.8,
    5.1,
    0.0,
    3.7,
    1.2,
    6.5,
    0.4,
    2.9,
    0.0,
    4.3
  };

  gfx.drawForLightBackground = false;
  const int x = 30;
  const int y = 75;
  const uint16_t width = 225;
  const uint16_t height = 125;
  const uint16_t start = 0;
  const uint16_t end = sizeof(data)/sizeof(data[0]);
  const float min = -1.0;
  const float max = 7.0;
  const uint16_t color = gfx.getRGB565FromRGB888(0, 255, 0);
  const bool drawBackground = true;

  tft.setCursor(x, y-11);
  tft.setTextColor(WHITE);
  tft.print("Daily rainfall in mm over 12 days");
  gfx.drawPointDiagram(x, y, width, height, data, start, end, min, max, color, drawBackground);
}

void loop() {
}