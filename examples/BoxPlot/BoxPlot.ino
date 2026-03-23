#include <Adafruit_TFTLCD.h>
#include <TFTGraph.h>

#define BLACK 0x0000
#define WHITE 0xFFFF

Adafruit_TFTLCD tft;
TFTGraph gfx(tft);

void setup() {
  tft.begin(0x9325);
  tft.reset();

  tft.fillScreen(BLACK);

  //Synthetic exam scores dataset:
  float data[] = {
    72.5, 88.0, 91.5, 67.0, 85.5,
    78.0, 94.0, 59.5, 73.0, 88.5,
    62.0, 79.5, 84.0, 90.0, 76.5,
    68.5, 81.0, 95.0, 70.0, 86.0
  };

  const int x = 64;
  const int y = 32;
  const uint16_t width = 100;
  const uint16_t height = 200;
  const uint16_t start = 0;
  const uint16_t end = sizeof(data)/sizeof(data[0]);
  const uint16_t color = 0xf800;
  const bool drawBackground = true;

  tft.setCursor(x, y-11);
  tft.setTextColor(WHITE);
  tft.print("Exam scores:");

  /*
  input:
  int x, int y, uint16_t width, uint16_t height, float data[], uint16_t start, uint16_t end, uint16_t color, bool drawBackground
  */

  gfx.drawBoxPlot(x, y, width, height, data, start, end, color, drawBackground);
}

void loop() {
}