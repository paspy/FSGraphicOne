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
#define NUM_PIXELS (RASTER_WIDTH*RASTER_HEIGHT)

struct Point {
	Point() {}
	Point(unsigned int _x, unsigned int _y) : x(_x), y(_y) {}
	unsigned int x, y;
};

unsigned int BackBuffer[NUM_PIXELS];

// function prototype
void ClearBuffer(unsigned int* _srcBuffer);
int Convert2Dto1D(const unsigned int _x, const unsigned int _y, const unsigned int _width);
int RandInRange(int _min, int _max);
unsigned int Lerp(unsigned int _A, unsigned int _B, float _ratio);
void DrawPoint(const unsigned int _x, const unsigned int _y, unsigned int *_buffer, unsigned int _color);
void DrawBresehamLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color);
void DrawMidpointLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color);
void DrawParametricLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color);

int main() {
	srand(unsigned int(time(nullptr)));

	RS_Initialize(RASTER_WIDTH, RASTER_HEIGHT);

	ClearBuffer(BackBuffer);

	// random 2500 Points

	for ( int i = 0; i < 2500; i++ ) {
		DrawPoint(RandInRange(0, RASTER_WIDTH - 1), RandInRange(0, RASTER_HEIGHT - 1), BackBuffer, 0xFFFFFFFF);
	}

	DrawBresehamLine(0, 100, 499, 400, BackBuffer, 0xFFFF0000);
	DrawMidpointLine(0, 110, 499, 410, BackBuffer, 0xFF00FF00);
	DrawParametricLine(0, 120, 499, 420, BackBuffer, 0xFF0000FF);

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

void DrawPoint(const unsigned int _x, const unsigned int _y, unsigned int *_buffer, unsigned int _color) {
	_buffer[Convert2Dto1D(_x, _y, RASTER_WIDTH)] = _color;
}

int Convert2Dto1D(const unsigned int _x, const unsigned int _y, const unsigned int _width) {
	assert(_x <= RASTER_HEIGHT&&_y <= RASTER_HEIGHT);
	return _y*_width + _x;
}

int RandInRange(int _min, int _max) {
	return _min + (rand() % (int)(_max - _min + 1));
}

int Lerp(int _A, int _B, float _ratio) {
	return (int)((((float)_B - (float)_A) * _ratio) + (float)_A);
}

void DrawBresehamLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color) {
	int deltaX = _x1 - _x0;
	int deltaY = _y1 - _y0;
	float error = 0;
	float slope = (float)deltaY / (float)deltaX;
	int y = _y0;
	for ( int x = _x0; x < _x1; x++ ) {
		DrawPoint(x, y, _buffer, _color);
		error = error + slope;
		if ( abs(error) >= 0.5f ) {
			y++;
			error -= 1.0f;
		}
	}
}

void DrawMidpointLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color) {
	int x = _x0;
	int y = _y0;
	int a = _y0 - _y1;
	int b = _x1 - _x0;
	int d = 2 * a + b;
	int d1 = 2 * a;
	int d2 = 2 * (a + b);
	DrawPoint(x, y, _buffer, _color);
	while ( x < _x1 ) {
		if ( d < 0 ) {
			x++, y++, d += d2;
		} else {
			x++, d += d1;
		}
		DrawPoint(x, y, _buffer, _color);
	}
}

void DrawParametricLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color) {
	int deltaX = _x1 - _x0;
	for ( int currX = _x0; currX < _x1; currX++ ) {
		float ratio = ((float)currX - (float)_x0) / (float)deltaX;
		int currY = Lerp(_y0, _y1, ratio);
		DrawPoint(currX, (int)floor(currY + 0.5f), _buffer, _color);
	}
}

