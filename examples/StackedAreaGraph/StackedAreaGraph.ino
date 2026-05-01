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

  //this data doesn't mean anything, only for demonstration purposes:
  float dataA[20] = {
    80, 82, 85, 88, 90, 92, 91, 89, 87, 86,
    88, 91, 94, 96, 95, 93, 90, 88, 85, 83
  };

  float dataB[20] = {
    70, 72, 74, 77, 79, 82, 86, 90, 91, 89,
    85, 82, 81, 83, 82, 80, 78, 76, 73, 71
  };

  float dataC[20] = {
    60, 61, 63, 65, 67, 68, 67, 66, 65, 64,
    65, 67, 69, 71, 70, 68, 66, 64, 62, 61
  };

  float dataD[20] = {
    50, 51, 52, 54, 55, 56, 55, 54, 53, 52,
    53, 54, 56, 58, 57, 56, 54, 53, 51, 50
  };

  gfx.drawForLightBackground = false;
  const int x = 40;
  const int y = 30;
  const uint16_t width = 270;
  const uint16_t height = 190;
  const int start = 0;
  const int end = 20;
  int min = 30;
  int max = 100;
  int lineThickness = 1;
  const uint16_t colorA = gfx.getRGB565FromRGB888(255, 0, 0);
  const uint16_t colorB = gfx.getRGB565FromRGB888(10, 10, 200);
  const uint16_t colorC = gfx.getRGB565FromRGB888(255, 200, 0);
  const uint16_t colorD = gfx.getRGB565FromRGB888(25, 255, 25);
  const bool drawBackground = true;
  const bool fill = true;

  gfx.drawLineDiagram(x, y, width, height, dataA, start, end, min, max, lineThickness, colorA, drawBackground, fill);
  //drawing the background is only necessary once:
  gfx.drawLineDiagram(x, y, width, height, dataB, start, end, min, max, lineThickness, colorB, !drawBackground, fill);
  gfx.drawLineDiagram(x, y, width, height, dataC, start, end, min, max, lineThickness, colorC, !drawBackground, fill);
  gfx.drawLineDiagram(x, y, width, height, dataD, start, end, min, max, lineThickness, colorD, !drawBackground, fill);

  //By combining filled line diagrams you can create a stacked area graph by doing something like this example.
}

void loop() {
}