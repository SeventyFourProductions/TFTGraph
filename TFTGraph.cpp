#include "Arduino.h"
#include "math.h"
#include "TFTGraph.h"
#include "InternalUtils.h"

bool TFTGraph::drawForLightBackground = false;

void TFTGraph::drawBarChart(int x, int y, uint16_t width, uint16_t height, float data[], float start, float end, uint16_t color, bool drawBackground){

	//cleaning values:
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

  	if(drawBackground) InternalUtils::drawDiagramBody(_gfx, x, y, width, height, min,max);

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

void TFTGraph::drawBoxPlot(int x, int y, uint16_t width, uint16_t height, float data[], uint16_t start, uint16_t end, uint16_t color, bool drawBackground){
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

	if(drawBackground) InternalUtils::drawDiagramBody(_gfx, x, y, width, height, minimum-flex, maximum+flex);

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

void TFTGraph::drawGauge(int x, int y, uint8_t r, float value, float min, float max, uint16_t color) {

	bool isLightBackground = InternalUtils::isLightBackground(_gfx, x, y);
    uint16_t drawColor = 0xFFFF;
    if(isLightBackground) drawColor = 0x0000;

	InternalUtils::drawHalfCircle(_gfx,x,y,r,drawColor);

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
  	_gfx.drawFastHLine(x-1,y,3,drawColor);
  	_gfx.drawFastVLine(x,y-1,3,drawColor);

  	float step = (max - min) / 8;
  	float currentStep = min;

  	//draw text:
  	for(int i = 26; i>9; i--){

    	bool offset = false;
    	if(i<=16){
    		offset = true;
    	}

    	//Only draw if "i" is an even number (drawing every iteration would get cluttered.)
    	if( i % 2 != 0) continue;

    	_gfx.setCursor(
    	  	x+InternalUtils::sind(i*10)*(r-5)+((String(currentStep).length()*2)*(offset*-1)),
    	  	y+InternalUtils::cosd(i*10)*(r-5)
		);
    	_gfx.setTextColor(drawColor);
    	_gfx.setTextSize(1);
    	_gfx.print(currentStep,0);
    	currentStep += step;
	}
}

void TFTGraph::drawLineDiagram(int x, int y, uint16_t width, uint16_t height, float data[], uint16_t start, uint16_t end, float min, float max, uint8_t lineThickness, uint16_t color, bool drawBackground, bool fill){

  	//"Cleaning" some values:
  	start = min(start, end);
  	min = min(min, max);
	lineThickness = max(1, lineThickness);

	if(drawBackground) InternalUtils::drawDiagramBody(_gfx, x, y, width, height,min, max);

  	int oldX = 0;
  	int oldY = 0;
  	int newX = 0;
  	int newY = 0;
  	//Iterating through data, drawing lines on screen:
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

			int yBottom = (y+height)-2;

			if(fill){
				//draw two triangles (a polygon or quart) to fill below the current iteration of line:
				_gfx.fillTriangle(
					oldX,
					oldY,
					newX,
					newY,
					oldX,
					yBottom,
					color
				);
				_gfx.fillTriangle(
					newX,
					newY,
					newX,
					yBottom,
					oldX,
					yBottom,
					color
				);
			}

			for(uint16_t i2 = 0; i2<lineThickness; i2++){

				if(i2 % 2 == 0) _gfx.drawLine(oldX,constrain(oldY+(i2/2), y, yBottom),newX,constrain(newY+(i2/2), y, yBottom),color);
				if(i2 % 2 != 0)	_gfx.drawLine(oldX,constrain(oldY-(i2/2), y, yBottom),newX,constrain(newY-(i2/2), y, yBottom),color);
			}
    	}
	}
}

void TFTGraph::drawPieChart(int x, int y, uint8_t r, float data[], uint16_t start, uint16_t end, uint8_t triangleWidth, uint16_t colors[], char names[][15], uint8_t selection, bool printNames) {
	//cleaning values:
	start = min(start, end);
	triangleWidth = max(1, triangleWidth);

	bool isLightBackground = InternalUtils::isLightBackground(_gfx, x, y);
    uint16_t drawColor = 0xFFFF;
    if(isLightBackground) drawColor = 0x0000;

	//Making slices stick out (explode):
	uint8_t explodeOffset = 1;
	bool explode = false;
	if(selection <= end && selection >= start){
		explode = true;
		explodeOffset = max(r*0.15, 1); 
	}

  	//sum all values:
  	float sum = 0.0;
  	for(int i = start; i < end; i++){
    	sum += data[i];
  	}

	int legendHeight = (end-start)*11;
	int legendTop = y-(legendHeight/2);
	int xPosLegend = x+r+8;
  	float currentAngle = 0.0;

  	for(int i = start; i<end; i++){
    	float angleAdd = 360.0 * (data[i] / sum);

    	//filling in colors:
		int currentTriAngle = triangleWidth;
    	for (int currentTriAngle = currentAngle; currentTriAngle < currentAngle+angleAdd; currentTriAngle += triangleWidth) {

			float cosValOffset = 0;
			float sinValOffset = 0;
			if(selection == i){
				float avgAng = (currentAngle+(currentAngle+angleAdd))/2.0;
				cosValOffset = InternalUtils::cosd(avgAng)*explodeOffset;
				sinValOffset = InternalUtils::sind(avgAng)*explodeOffset;
			}

			float cosVal1 = InternalUtils::cosd(currentTriAngle);
			float sinVal1 = InternalUtils::sind(currentTriAngle);

			float cosVal2 = InternalUtils::cosd(currentTriAngle+min(triangleWidth, (currentAngle+angleAdd)-currentTriAngle));
			float sinVal2 = InternalUtils::sind(currentTriAngle+min(triangleWidth, (currentAngle+angleAdd)-currentTriAngle));
			

			_gfx.fillTriangle(
				x+cosValOffset,
				y+sinValOffset,
				x+cosVal1*r+cosValOffset,
				y+sinVal1*r+sinValOffset,
				x+cosVal2*r+cosValOffset,
				y+sinVal2*r+sinValOffset,

				colors[i]
			);
    	}

		currentAngle += angleAdd;
		if(!printNames) continue;

		int yPosLegend = legendTop+(i*11);

    	//first drawing square of color:
    	_gfx.fillRect(
			xPosLegend,
			yPosLegend,
			4,
			4,
			colors[i]
		);

		if(selection == i){
			_gfx.drawRect(
				xPosLegend-1,
				yPosLegend-1,
				6,
				6,
				colors[i]
			);	
		}

    	//drawing text:
		_gfx.setTextWrap(false);
    	_gfx.setCursor(xPosLegend+6 , yPosLegend-2);
    	_gfx.setTextSize(1);
		if(selection == i){
			_gfx.setTextColor(colors[i]);
		}else{
			_gfx.setTextColor(drawColor);
		}
    	float percent = (angleAdd / 360.0) * 100.0;

		bool hasName = names[i][0] != '\0';
		if(hasName){
			_gfx.print(names[i]);
			_gfx.setTextColor(0xdefb);
			_gfx.print(" (");
		}
		_gfx.print(data[i],0);
		_gfx.print("/");
    	_gfx.print(percent, 1);
    	_gfx.print("%");
		if(hasName) _gfx.print(")");
	}
}

void TFTGraph::drawPointDiagram(int x, int y, uint16_t width, uint16_t height, float data[], uint16_t start, uint16_t end, float min, float max, uint16_t color, bool drawBackground){
  	//"Cleaning" some values:
  	start = min(start, end);
  	min = min(min, max);

  	if(drawBackground) InternalUtils::drawDiagramBody(_gfx, x, y, width, height, min, max);

  	//iterating through data, drawing points on screen:
  	for(uint16_t i = start; i<end; i++){

    	// ( value-minimum ) / ( maximum-minimum )
    	float relativePos = 1.0-(constrain((data[i]-min)/(max-min),0.0,1.0));
      	/*
      	x + 1 is the starting position.
      	(int) turns everything in the next parantheses into an integer, essentially cutting off the decimal point. This is why you have (+0.5) at the end, so it essentially acts as rounding.
      	(float) turns the next parantheses, (i-start) into floating point numbers, allowing for precise calculation.
      	(i - start)*(width - 2) is basically the position of the point. width has subtracted 2 in order to cut off 2 pixels.
      	/ (end-start) is how long the graph is, it needs to divide to see how much space should be between each point.
      	*/
		int xPoint = x + 1 + (int)(((float)(i - start) * (width - 2)) / ((end - start) - 1) + 0.5);
		int yPoint = y+7+((height-10)*relativePos);

		_gfx.drawFastVLine(xPoint, yPoint-1, 3, color);
		_gfx.drawFastHLine(xPoint-1, yPoint, 3, color);
  	}
}

void TFTGraph::drawScatterPlot(int x, int y, uint16_t width, uint16_t height, float data[][3], int start, int end, uint16_t colors[], bool drawBackground){

	bool isLightBackground = InternalUtils::isLightBackground(_gfx, x, y);
    uint16_t drawColor = 0xFFFF;
    if(isLightBackground) drawColor = 0x0000;

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

  	if(drawBackground) InternalUtils::drawScatterBody(_gfx, x,y, width, height, xMin, xMax, yMin, yMax);

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
  	if(drawBackground) _gfx.drawRect(x,y,width+2,height+2,drawColor);
}

uint16_t TFTGraph::getRGB565FromRGB888(uint8_t r, uint8_t g, uint8_t b){
	//Noticing some inaccuracies with the colour ((255,0,0) doesn't return brightest red.) due to how this is implemented, unaware if there exists a better solution.
	uint8_t R5 = (r >> 3);
	uint8_t G6 = (g >> 2);
	uint8_t B5 = (b >> 3);

	return ((R5 << 11) | (G6 << 5)) | B5;
}