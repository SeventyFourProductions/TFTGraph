#include <Adafruit_TFTLCD.h>
#include <TFTGraph.h>

#define BLACK 0x0000
#define WHITE 0xFFFF

Adafruit_TFTLCD tft;
TFTGraph gfx(tft);

void setup(){
  tft.begin(0x9325);
  tft.reset();
  tft.setRotation(1);

  tft.fillScreen(BLACK);

  //data being presented: Time usage in a day (in hours)
  float data[] = {8,8,4,2,2};
  uint16_t dataSize = sizeof(data) / sizeof(data[0]);

  //The list of names and colors must be in the same order as the data the name and color represents:
  uint16_t colors[] = {0x0015, 0xFD20, 0x07E0, 0xFFE0, 0x8410};
  char names[][15] = {
    "Sleep",
    "Work/school",
    "Leisure",
    "Eating",
    "Other"
  };

  /*
  <int x, int y, uint8_t radius, float data[], uint16_t start, uint16_t end, int triangleWidth, uint16_t colors, char names[][15], uint8_t selection, bool printNames>

  "triangleWidth" describes how wide (in degrees) each triangle that makes up a full pie chart is. Bigger numbers draw the pie chart faster, in exchange for quality.
  If "selection" is between the "start" and "end" value the given pie slice will be highlighted and selected. If the value is outside of the bounds then none of the slices are selected.
  */

  const int x = 100;
  const int y = 100;
  const uint8_t r = 64;
  const uint16_t start = 0;
  const uint8_t selection = 1;
  const bool printNames = true;
  const int triangleWidth = 10;

  tft.setCursor(40,20);
  tft.setTextColor(WHITE);
  tft.print("Time usage in a day");

  gfx.drawPieChart(x, y, r, data, start, dataSize, triangleWidth, colors, names, selection, printNames);
}

void loop(){
}