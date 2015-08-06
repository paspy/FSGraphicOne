#include <iostream>
#include <utility>
#include <ctime>
#include <vector>
#include "RasterSurface.h"
#include "XTime.h"
#include "tiles_12.h"	// input tile file
#include "fire_01.h"	// input particle file

#include <cassert>


// grass coord = 288*128 to 319*159

#define RASTER_WIDTH 500
#define RASTER_HEIGHT 500
#define RASTER_DEPTH 5
#define NUM_PIXELS (RASTER_WIDTH*RASTER_HEIGHT*RASTER_DEPTH)

struct Point {
	Point() {}
	Point(unsigned int _x, unsigned int _y) : x(_x), y(_y) {}
	unsigned int x, y;
};

unsigned int BackBuffer[NUM_PIXELS];

// function prototype
void ClearBuffer(unsigned int* _srcBuffer);
int Convert3Dto1D(const unsigned int _x, const unsigned int _y, const unsigned int _z, const unsigned int _width);
int RandInRange(int _min, int _max);
unsigned int LerpARGB(unsigned int _A, unsigned int _B, float _ratio);

int main() {
	srand(unsigned int(time(nullptr)));

	RS_Initialize(RASTER_WIDTH, RASTER_HEIGHT);

	ClearBuffer(BackBuffer);

	while (RS_Update(BackBuffer, NUM_PIXELS)) {

	}

	RS_Shutdown();

	return 0;
}

void ClearBuffer(unsigned int* _srcBuffer) {
	for ( int i = 0; i < NUM_PIXELS; i++ ) {
		_srcBuffer[i] = 0xFF000000;
	}
}

int Convert3Dto1D(const unsigned int _x, const unsigned int _y, const unsigned int _z, const unsigned int _width) {
	assert(_x < RASTER_HEIGHT&&_y < RASTER_HEIGHT&&_z < RASTER_DEPTH);
	return _y*_width + _x;
}

int RandInRange(int _min, int _max) {
	return _min + (rand() % (int)(_max - _min + 1));
}

unsigned int LerpARGB(unsigned int _A, unsigned int _B, float _ratio) {
	return (unsigned int)((((int)_B - (int)_A) * _ratio) + _A);
	//return ((_B - _A) * _ratio) + _A;
}

/*
function line(x0, x1, y0, y1)
boolean steep := abs(y1 - y0) > abs(x1 - x0)
if steep then
swap(x0, y0)
swap(x1, y1)
if x0 > x1 then
swap(x0, x1)
swap(y0, y1)

int deltax := x1 - x0
int deltay := abs(y1 - y0)
int error := deltax / 2
int ystep
int y := y0
if y0 < y1 then ystep := 1 else ystep := -1
for x from x0 to x1
if steep then plot(y,x) else plot(x,y)
error := error - deltay
if error < 0 then
y := y + ystep
error := error + deltax
*/
void DrawBresehamLine(int _x0, int _y0, int _x1, int _y1) {
	bool steep = abs(_y1 - _y0) > abs(_x1 - _x0);
	if ( steep ) {
		std::swap(_x0, _y0);
		std::swap(_x1, _y1);
	}
	if ( _x0 > _x1 ) {
		std::swap(_x0, _x1);
		std::swap(_y0, _y1);
	}
	int deltaX = _x1 - _x0;
	int deltaY = abs(_y1 - _y0);
	int error = deltaX / 2;
	int yStep;
	int y = _y0;
	if ( _y0 < _y1 ) {
		yStep = 1;
	} else {
		yStep = -1;
	}
	int x = 0;
	for ( x = _x0; x < _x1; x++ ) {

	}
}