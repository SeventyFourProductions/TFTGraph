#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TFTGraph.h>

#define BLACK 0x0000
#define WHITE 0xFFFF
#define LIGHTGRAY 0xC618
#define GRAY 0x8410
#define DARKGRAY 0x4208

Adafruit_TFTLCD tft;
TFTGraph gfx(tft);

void setup() {
  tft.begin(0x9325);

  tft.reset();
  tft.fillScreen(0x0000);
  tft.setRotation(1);
}

float value = 0;
bool dir = true;

void loop() {
  if(value >= 5) dir = false;
  if(value <=-5) dir = true;
  if(dir) value += 0.5;
  if(!dir) value -= 0.5;

  const int x = 160;
  const int y = 150;
  const uint8_t r = 128;
  const float min = -5;
  const float max = 5;
  const uint16_t color = 0x37e0;

  tft.fillScreen(BLACK);

  /*
  input:
  int x, int y, uint8_t radius, float value, float min, float max, uint16_t color
  */
  gfx.drawGauge(x, y, r, value, min, max, color);

  delay(50);
}