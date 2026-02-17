#include <math.h>
#include <Adafruit_TFTLCD.h>
#include <TFTGraph.h>

#define BLACK 0x0000
#define WHITE 0xFFFF
#define LIGHTGRAY 0xC618
#define GRAY 0x8410
#define DARKGRAY 0x4208
#define YELLOW 0xf7e0
#define RED 0xf800
#define ORANGE 0xfe00
#define GREEN 0x07e2
#define CYAN 0x07df

Adafruit_TFTLCD tft;
TFTGraph gfx(tft);

void setup() {
  tft.begin(0x9325);
  tft.reset();
  tft.setRotation(1);

  tft.fillScreen(0x0000);
}

void loop() {
  float data[] = {56.7,34.6,23.4,45.2,12.4,65.2,56.0,34.4,23.0,56.1};

  tft.fillScreen(BLACK);
  gfx.drawPointDiagram(50,50,150,150, data,0,sizeof(data)/sizeof(data[0]),12.4,70.0, YELLOW);
  delay(5000);
  tft.fillScreen(BLACK);
  gfx.drawLineDiagram(50,50,150,150, data,0,sizeof(data)/sizeof(data[0]),12.4,70.0, CYAN);
  delay(5000);
  tft.fillScreen(BLACK);
  gfx.drawBarChart(50, 50, 150, 150, data, 0, sizeof(data)/sizeof(data[0]), GREEN);
  delay(5000);
  tft.fillScreen(BLACK);
  gfx.drawGauge(150, 150, 128, 69.0, 0, 100, YELLOW);
  delay(5000);

  uint16_t colors[] = {YELLOW, RED, ORANGE, GREEN, CYAN, YELLOW, RED, ORANGE, GREEN, CYAN};
  char names[][15] ={
    "Bernt",
    "Arne",
    "Bjorn",
    "Karl",
    "Ole",
    "Augustus",
    "Frida",
    "Bolla",
    "TEMU",
    "Maria"
  };
  tft.fillScreen(BLACK);
  gfx.drawPieChart(150, 100, 48, data, 0, 4, colors, names);
  delay(5000);

  float data2[][3] = {
    // Group 0 (lower-left-ish, spreads upward)
    {1.1, 2.3, 0},
    {1.4, 2.7, 0},
    {1.8, 3.0, 0},
    {2.0, 3.4, 0},
    {1.6, 3.1, 0},
    {2.2, 3.6, 0},
    {1.9, 2.9, 0},

    // Group 1 (overlaps with group 0 & 2)
    {2.8, 2.4, 1},
    {3.1, 2.7, 1},
    {3.4, 3.0, 1},
    {3.0, 2.9, 1},
    {3.6, 3.2, 1},
    {3.2, 2.6, 1},
    {3.8, 3.5, 1},

    // Group 2 (upper-middle, overlaps with group 1)
    {3.2, 3.8, 2},
    {3.6, 4.1, 2},
    {3.9, 4.4, 2},
    {4.1, 4.0, 2},
    {3.7, 4.2, 2},
    {4.4, 4.6, 2},
    {3.5, 3.9, 2},

    // Group 3 (right side, overlaps with group 2)
    {4.8, 3.6, 3},
    {5.1, 3.9, 3},
    {5.4, 4.2, 3},
    {5.0, 3.8, 3},
    {5.6, 4.5, 3},
    {5.2, 4.0, 3}
};
  tft.fillScreen(BLACK);
  gfx.drawScatterPlot(50, 50, 200, 150, data2, 0, sizeof(data2)/sizeof(data2[0]), colors);
  delay(5000);

  //Both scatter diagram and a line diagram:
  tft.fillScreen(BLACK);
  gfx.drawScatterPlot(50, 50, 200, 50, data2, 0, sizeof(data2)/sizeof(data2[0]), colors);
  gfx.drawLineDiagram(50,125,150,100, data,0,sizeof(data)/sizeof(data[0]),12.4,70.0, CYAN);
  gfx.drawPieChart(200, 100, 48, data, 0, 4, colors, names);
  delay(15000);
}

