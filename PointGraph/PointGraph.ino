#include <math.h>
#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>

#define BLACK 0x0000
#define WHITE 0xFFFF
#define LIGHTGRAY 0xC618
#define GRAY 0x8410
#define DARKGRAY 0x4208

Adafruit_TFTLCD tft;

void setup() {
  Serial.begin(9600);

  tft.begin(0x9325);

  tft.reset();
  tft.fillScreen(0x0000);

  float data[50];
  fillRandomFloatArray(data, 50);


  drawPointDiagram(30, 20, 200, 64, data, 0, 50, 0xF450);
}

void loop() {
}

void fillRandomFloatArray(float arr[], int size) {
  for (int i = 0; i < size; i++) {
    arr[i] = sin(i/10.0)*50;  // 0.00 → 100.00
  }
}

void drawPointDiagram(int x, int y, int width, int height, float data[], int start, int end, uint16_t color){

  for(int i = 0; i<height/10; i++){
    tft.drawFastHLine(x, y+(i*10),width, GRAY);
  }

  /*tft.drawFastVLine(x-2,y-2,height+6,WHITE);
  tft.drawFastVLine(x-3,y-2,height+6,WHITE);*/

  //retrieving the highest and lowest number in the array:
  float max_num = data[start];
  float min_num = data[start];
  for(int i = start; i<end; i++){
    if(max_num < data[i]){
      max_num = data[i];
      continue;
    } else if(min_num > data[i]){
      min_num = data[i];
    }else{
      continue;
    }
  }

  //iterating through data, drawing points on screen:
  for(int i = start; i<end; i++){

    // ( value-minimum ) / ( maximum-minimum )
    float relativePos = (data[i]-min_num)/(max_num-min_num);
    Serial.println(relativePos);

    tft.fillCircle(
      x + 4 + ((i - start) * width) / (end - start),
      y+7+((height-10)*relativePos),
      1,
      color
    );
  }

  //drawing bounding box:
  tft.drawRect(x,y,width+2,height,WHITE);

  Serial.println(max_num);
}