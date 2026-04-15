#include <Adafruit_TFTLCD.h>
#include <TFTGraph.h>

Adafruit_TFTLCD tft;
TFTGraph gfx(tft);

#define BLACK gfx.getRGB565FromRGB888(0, 0, 0)
#define WHITE gfx.getRGB565FromRGB888(255, 255, 255)
#define LIGHTGRAY gfx.getRGB565FromRGB888(190, 190, 190)
#define GRAY gfx.getRGB565FromRGB888(100, 100, 100)
#define DARKGRAY gfx.getRGB565FromRGB888(50, 50, 50)
#define YELLOW gfx.getRGB565FromRGB888(255, 255, 0)
#define RED gfx.getRGB565FromRGB888(255, 0, 0)
#define ORANGE gfx.getRGB565FromRGB888(255, 165, 0)
#define GREEN gfx.getRGB565FromRGB888(0, 255, 0)
#define CYAN gfx.getRGB565FromRGB888(0, 255, 255)

void setup() {
  tft.begin(0x9325);
  tft.reset();
  tft.setRotation(1);

  tft.fillScreen(BLACK);
}

void loop() {
  float data[] = {56.7,34.6,23.4,45.2,12.4,65.2,56.0,34.4,23.0,56.1};

  tft.fillScreen(BLACK);
  gfx.drawPointDiagram(50,50,150,150, data,0,sizeof(data)/sizeof(data[0]),12.4,70.0, YELLOW, true);
  delay(2500);
  tft.fillScreen(BLACK);
  gfx.drawLineDiagram(50,50,150,150, data,0,sizeof(data)/sizeof(data[0]),12.4,70.0, 2, CYAN, true, false);
  delay(2500);
  tft.fillScreen(BLACK);
  gfx.drawBarChart(50, 50, 150, 150, data, 0, sizeof(data)/sizeof(data[0]), GREEN, true);
  delay(2500);
  tft.fillScreen(BLACK);
  gfx.drawGauge(150, 150, 128, 69.0, 0, 100, YELLOW);
  delay(2500);
  tft.fillScreen(BLACK);
  gfx.drawBoxPlot(32, 32, 100, 200, data, 0, sizeof(data)/sizeof(data[0]), WHITE, true);
  delay(2500);

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
  gfx.drawPieChart(150, 100, 48, data, 0, 4, 5, colors, names, 100, true);
  delay(2500);

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
  gfx.drawScatterPlot(50, 50, 200, 150, data2, 0, sizeof(data2)/sizeof(data2[0]), colors, true);
  delay(2500);

  //Scatter diagram, line diagram and a pie chart at the same time:
  tft.fillScreen(BLACK);
  gfx.drawScatterPlot(50, 50, 200, 50, data2, 0, sizeof(data2)/sizeof(data2[0]), colors, true);
  gfx.drawLineDiagram(50,125,150,100, data,0,sizeof(data)/sizeof(data[0]),12.4,70.0, 2, CYAN, true, false);
  gfx.drawPieChart(150, 100, 48, data, 0, 4, 5, colors, names, 2, false);
  delay(7000);
}

