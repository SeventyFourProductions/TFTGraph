#include <math.h>
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
}

void loop() {
  tft.fillScreen(BLACK);

  float data[] = {10, 20, 32, 25, 15, 3, 2, 30, 23, 12, 17, 26, 30, 25, 15, 13, 24, 34, 26, 15};

  gfx.drawBoxPlot(64, 32, 100, 200, data, 0, sizeof(data)/sizeof(data[0]), 0xf800);

  delay(5000);
}