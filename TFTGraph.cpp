#include "Arduino.h"
#include "math.h"
#include "TFTGraph.h"
#include "InternalUtils.h"

void TFTGraph::drawGauge(int x, int y, uint8_t r, float value, float min, float max, uint16_t color) {
	InternalUtils::drawHalfCircle(_gfx,x,y,r,0xFFFF);

  	//Drawing gauge line:
  	int Ang = 260 + ((value - min) * (100 - 260)) / (max - min);
  	_gfx.drawLine(
    	x,
    	y,
    	x+InternalUtils::sind(Ang)*(r-(r/5)),
    	y+InternalUtils::cosd(Ang)*(r-(r/5)),
    	color
	);

  	//draw base of needle (simple point):
  	_gfx.drawFastHLine(x-1,y,3,0xFFFF);
  	_gfx.drawFastVLine(x,y-1,3,0xFFFF);

  	float step = (max - min) / 8;
  	float currentStep = min;

  	//draw text:
  	for(int i = 26; i>9; i--){

    	bool offset = false;
    	if(i<=16){
    		offset = true;
    	}

    	//Only draw if "i" is an even number (drawing every iteration would get a bit much.)
    	if( i % 2 != 0) continue;

    	_gfx.setCursor(
    	  	x+InternalUtils::sind(i*10)*(r-5)+((String(currentStep).length()*2)*(offset*-1)),
    	  	y+InternalUtils::cosd(i*10)*(r-5)
		);
    	_gfx.setTextColor(0xFFFF);
    	_gfx.setTextSize(1);
    	_gfx.print(currentStep,0);
    	currentStep += step;
	}
}

void TFTGraph::drawPointDiagram(int x, int y, uint16_t width, uint16_t height, float data[], uint16_t start, uint16_t end, float min, float max, uint16_t color){
  	//"Cleaning" some values:
  	start = min(start, end);
  	min = min(min, max);

  	InternalUtils::drawDiagramBody(_gfx, x, y, width, height, min, max);

  	//iterating through data, drawing points on screen:
  	for(uint16_t i = start; i<end; i++){

    	// ( value-minimum ) / ( maximum-minimum )
    	float relativePos = 1.0-(constrain((data[i]-min)/(max-min),0.0,1.0));

    	_gfx.fillCircle(
      		/*
      		x + 1 is the starting position.
      		(int) turns everything in the next parantheses into an integer, essentially cutting off the decimal point. This is why you have (+0.5) at 		the end, so it essentially acts as rounding.
      		(float) turns the next parantheses, (i-start) into floating point numbers, allowing for precise calculation.
      		(i - start)*(width - 2) is basically the position of the point. width has subtracted 2 in order to cut off 2 pixels.
      		/ (end-start) is how long the graph is, it needs to divide to see how much space should be between each point.
      		*/
      		x + 1 + (int)(((float)(i - start) * (width - 2)) / ((end - start) - 1) + 0.5),
      		y+7+((height-10)*relativePos),
      		1,
      		color
    	);
  	}
}

void TFTGraph::drawLineDiagram(int x, int y, uint16_t width, uint16_t height, float data[], uint16_t start, uint16_t end, float min, float max, uint16_t color){

  	//"Cleaning" some values:
  	x = max(0,x);
  	y = max(0,y);
  	width = max(5,width);
  	height = max(5,height);
  	start = min(start, end);
  	min = min(min, max);

	InternalUtils::drawDiagramBody(_gfx, x, y, width, height,min, max);

  	int oldX = 0;
  	int oldY = 0;
  	int newX = 0;
  	int newY = 0;
  	//iterating through data, drawing points on screen:
  	for(uint16_t i = start; i<end; i++){
    	// ( value-minimum ) / ( maximum-minimum )
    	float relativePos = 1.0-(constrain((data[i]-min)/(max-min),0.0,1.0));

    	if(i == start){
      		newX = x + 1 + (int)(((float)(i - start) * (width - 2)) / ((end - start) - 1) + 0.5);
      		newY = y+7+((height-10)*relativePos);
    	}else{
      		oldX = newX;
      		oldY = newY;
      		newX = x + 1 + (int)(((float)(i - start) * (width - 2)) / ((end - start) - 1) + 0.5);
      		newY = y+7+((height-10)*relativePos);

      		_gfx.drawLine(oldX,oldY,newX,newY,color);
      		_gfx.drawLine(oldX,oldY-1,newX,newY-1,color);
    	}
	}
}

void TFTGraph::drawPieChart(int x, int y, uint8_t r, float data[], int start, int end, uint16_t colors[], char names[][15]) {

  	//sum all values:
  	float sum = 0.0;
  	for(int i = start; i < end; i++){
    	sum += data[i];
  	}

  	float currentAngle = 0.0;

  	for(int i = start; i<end; i++){
    	float angleAdd = 360.0 * (data[i] / sum);

    	/*gfx.drawLine(x,y,
    	x+cosd(currentAngle)*r, y+sind(currentAngle)*r,
    	colors[i]);*/

    	//filling in colors:
    	//MAJOR OPTIMIZATION IS NEEDED HERE:
    	/*
    	Ideas:
    	- Draw only a quarter of the circle at once depending on the angle.
    	- Use fastHLine and fastVLine(No idea how to implement this.)
    	- Artificially decrease resolution (have 2x2 pixels instead of 1x1)
    	*/
    	for (int i2 = 0; i2 < r * 2; i2++) {
      		for (int i3 = 0; i3 < r * 2; i3++) {
        		//skipping half of the pixels to speed up drawing:
        		if ((i2 + i3) % 2 == 0) continue;
	
        		//getting current coordinates relative to x and y center:
        		int px = (x - r) + i2;
        		int py = (y - r) + i3;
	
        		int dx = px - x;
        		int dy = py - y;
	
        		float distance = sqrtf(dx * dx + dy * dy);
        		//if distance is over, skip this iteration:
        		if (distance > r){
          			continue;
        		}

        		//calculating angle from center to current pixel:
        		float angle = atan2(dy, dx) * 180.0 / PI;
        		if (angle < 0){
          			angle += 360;
        		}
        
        		//if angle is within slice, and the distance is within the circle, draw a pixel:
        		if (angle >= currentAngle && angle < currentAngle + angleAdd) {
          			_gfx.drawPixel(px, py, colors[i]);
    			}
    		}
    	}

    	//first drawing square of color:
    	_gfx.fillRect(x+r+8,(y-(r-10))+((i-start)*11),4,4,colors[i]);

    	//drawing text:
		_gfx.setTextWrap(false);
    	_gfx.setCursor(x+r+14,((y-(r-10))-2)+((i-start)*11));
    	_gfx.setTextSize(1);
    	_gfx.setTextColor(0xFFFF);
    	float percent = (angleAdd / 360.0) * 100.0;
		if(names[i][0] != '\0'){
			_gfx.print(names[i]);
			_gfx.print(" (");
		}
    	_gfx.print(percent, 1);
    	_gfx.print("%");
		if(names[i][0] != '\0') _gfx.print(")");

    	currentAngle += angleAdd;
	}
	//draw bounding circle:
	//gfx.drawCircle(x,y,r,0xFFFF);
}

/*void TFTGraph::drawPieChart(int x, int y, uint8_t r, float data[], int start, int end, uint16_t colors[], char names[][15]){
	//sum all values:
  	float sum = 0.0;
  	for(int i = start; i < end; i++){
    	sum += data[i];
  	}

  	float currentAngle = 0.0;

  	for(int i = start; i<end; i++){
		float angleAdd = 360.0 * (data[i] / sum);

		float dx1 = InternalUtils::cosd(currentAngle);
		float dy1 = InternalUtils::sind(currentAngle);
		float dx2 = InternalUtils::cosd(currentAngle + angleAdd);
		float dy2 = InternalUtils::sind(currentAngle + angleAdd);

		//Only scanning the lines where the angles actually are:
		
		int startYPos = y-r;
		int endYPos = y+r;
		if(currentAngle < 180 && currentAngle+angleAdd < 180) startYPos = y;
		if(currentAngle > 180 && currentAngle+angleAdd > 180) endYPos = y;
		
		for(int i2 = startYPos; i2<endYPos; i2++){
			int dy = i2 - y;
			float xExtent = sqrt(r*r - dy*dy);
			
			//start ray:
			float t1 = (i2 - y) / dy1;
			float x1 = x + t1 * dx1;

			//end ray:
			float t2 = (i2 - y) / dy2;
			float x2 = x + t2 * dx2;

			if (fabs(dy1) > 0.0001) {
    			float t1 = dy / dy1;
    			x1 = x + t1 * dx1;
			}

			if (fabs(dy2) > 0.0001) {
    			float t2 = dy / dy2;
    			x2 = x + t2 * dx2;
			}

			int xs = max(x - xExtent, min(x1, x2));
			int xe = min(x + xExtent, max(x1, x2));

			//draw line:
			_gfx.drawFastHLine(
				xs,
				i2,
				xe-xs,
				colors[i]
			);
		}
		currentAngle += angleAdd;
	}
}*/

void TFTGraph::drawBarChart(int x, int y, uint16_t width, uint16_t height, float data[], float start, float end, uint16_t color){

  start = min(start, end);

  //determing maximum and minimum number:
  float max = data[0];
  float min = 0;
  for(int i = start; i<(end-start); i++){
    if(data[i] > max){
      max = data[i];
    }
	if(data[i] < min){
      min = data[i];
    }
  }

  //add 10% padding if bar chart goes below 0 (to avoid the minimum value bar simply not rendering)
  if(min < 0){
    min -= (max-min)*0.1;
  }

  InternalUtils::drawDiagramBody(_gfx, x, y, width, height, min,max);

  //iterating through data values:
  for(int i = start; i<end; i++){
    float relativePos = 1.0-(constrain((data[i]-min)/(max-min),0.0,1.0));

    _gfx.fillRect(
      x + (i * (width / (end-start))),
      y+7+((height-10)*relativePos),
      (width / (end-start))*0.6,
      (y + height-1) - (y+7+((height-10)*relativePos)),
      color
    );
  }
}

void TFTGraph::drawScatterPlot(int x, int y, uint16_t width, uint16_t height, float data[][3], int start, int end, uint16_t colors[]){

  //find max and min:
  float xMax = data[0][0];
  float yMax = data[0][1];
  float xMin = data[0][0];
  float yMin = data[0][1];
  for(int i = start; i<end; i++){
    if(data[i][0] > xMax){
      xMax = data[i][0];
    }
    if(data[i][1] > yMax){
      yMax = data[i][1];
    }
    if(data[i][0] < xMin){
      xMin = data[i][0];
    }
    if(data[i][1] < yMin){
      yMin = data[i][1];
    }
  }

  InternalUtils::drawScatterBody(_gfx, x,y, width, height, xMin, xMax, yMin, yMax);

  //iterating through data points:
  for(int i = start; i<end; i++){
    //normalizing position:
    float normalXPos = (data[i][0] - xMin) / (xMax - xMin);
    float normalYPos = 1.0-((data[i][1] - yMin) / (yMax - yMin));
    
    //horizontal line:
    _gfx.drawFastHLine(
      x+(width*normalXPos)-1,
      y+(height*normalYPos),
      3,
      colors[(int)(data[i][2])]
    );

    //vertical line:
    _gfx.drawFastVLine(
      x+(width*normalXPos),
      y+(height*normalYPos)-1,
      3,
      colors[(int)(data[i][2])]
    );
  }

  _gfx.drawRect(x,y,width+2,height+2,0xFFFF);
}

void TFTGraph::drawBoxPlot(int x, int y, uint16_t width, uint16_t height, float data[], int start, int end, uint16_t color){
	//sorting given array:
	InternalUtils::sortArray(data, start, end);

	//calculating minimum, maximum, sum, average, mean, lower quartile and higher quartile
	float sum = 0;
	float minimum = data[0];
	float maximum = data[end-1];
	for(int i = start; i<end; i++){
		sum += data[i];
	}

	//determining amount of empty space at top or bottom:
	float flex = (maximum-minimum)*0.1;

	InternalUtils::drawDiagramBody(_gfx, x, y, width, height, minimum-flex, maximum+flex);

	float mean = sum/(end-start);

	//getting median and Lower and Higher Quartile:
	float median = InternalUtils::getMedian(data, start, end);

	int mid = start + (end-start) / 2;
	float LQ = 0;
	float HQ = 0;
	if((end-start) % 2 == 0){
		LQ = InternalUtils::getMedian(data,start,mid);
		HQ = InternalUtils::getMedian(data,mid,end);
	}else{
		LQ = InternalUtils::getMedian(data,start,mid);
		HQ = InternalUtils::getMedian(data,mid+1,end);
	}

	//drawing the box:
	//drawing the whisker (the big vertical line in the middle)
	float relativeMaximum = 1.0-(constrain((maximum-(minimum-flex))/((maximum+flex)-(minimum-flex)),0.0,1.0));
	float relativeMinimum = 1.0-(constrain((minimum-(minimum-flex))/((maximum-flex)-(minimum-flex)),0.0,1.0));
	for(int i = 0; i<2/*how many pixels thick the line should be*/; i++){
		_gfx.drawFastVLine(
			x+(width/2)+i,
			y+(relativeMaximum*height),
			(relativeMinimum*height)-(relativeMaximum*height),
			color
		);
	}

	//drawing whisker caps:
	//top whisker cap:
	int whiskerWidth = width/4;
	int whiskerX = x+((width-whiskerWidth)/2);
	_gfx.drawFastHLine(
		whiskerX,
		y+(relativeMaximum*height),
		whiskerWidth,
		color
	);
	_gfx.drawFastHLine(
		whiskerX,
		y+(relativeMinimum*height)-1,
		whiskerWidth,
		color
	);

	//drawing box:
	/*
		1. line in the middle is median
		2. top of box is the top quarter, while the bottom is the bottom quarter
	*/
	float relativeLQ = 1.0-(constrain((LQ-(minimum-flex))/((maximum+flex)-(minimum-flex)),0.0,1.0));
	float relativeHQ = 1.0-(constrain((HQ-(minimum-flex))/((maximum+flex)-(minimum-flex)),0.0,1.0));

	int boxTopY = y + (relativeHQ * height);
	int boxBottomY = y + (relativeLQ * height);

	int boxWidth = width/2;
	int boxX = x+((width-boxWidth)/2);

	_gfx.fillRect(
		boxX,
		boxTopY,
		boxWidth,
		boxBottomY-boxTopY,
		color
	);

	//drawing line where Median is:
	float relativeMedian = 1.0-(constrain((median-(minimum-flex))/((maximum+flex)-(minimum-flex)),0.0,1.0));
	uint16_t medianColor = 0x0000;
	if(color == 0x0000) medianColor = 0xFFFF;

	for(int i = 0; i<2; i++){
		_gfx.drawFastHLine(
			boxX,
			y+(relativeMedian*height)+i,
			boxWidth,
			medianColor
		);
	}
}