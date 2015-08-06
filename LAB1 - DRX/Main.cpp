#include <iostream>
#include <utility>
#include <ctime>
#include <cstdio>
#include "RasterSurface.h"
#include "XTime.h"
#include "tiles_12.h"	// input tile file
#include "fire_01.h"	// input particle file

#include <cassert>


// grass coord = 288*128 to 319*159

#define RASTER_WIDTH 500
#define RASTER_HEIGHT 500
#define NUM_PIXELS (RASTER_WIDTH*RASTER_HEIGHT)

#define VKNUM_1 0x31
#define VKNUM_2 0x32
#define VKNUM_3 0x33
#define VKNUM_4 0x34


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
int Lerp_(unsigned int _A, unsigned int _B, float _ratio);
unsigned int ColorLerp(unsigned int _A, unsigned int _B, float _ratio);
void DrawPoint(const unsigned int _x, const unsigned int _y, unsigned int *_buffer, unsigned int _color);
void DrawBresehamLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color);
void DrawMidpointLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color);
void DrawParametricLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _StartColor, unsigned int _EndColor);

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
	DrawParametricLine(0, 120, 499, 420, BackBuffer, 0xFFFF00FF, 0xFF00FFFF);

	bool keyPressed = false;

	while (RS_Update(BackBuffer, NUM_PIXELS)) {
		if ( GetAsyncKeyState(VKNUM_1) && !keyPressed ) {
			ClearBuffer(BackBuffer);
			int _x0 = RandInRange(1, RASTER_WIDTH - 1);
			int _x1 = RandInRange(1, RASTER_WIDTH - 1);
			int _y0 = RandInRange(1, RASTER_WIDTH - 1);
			int _y1 = RandInRange(1, RASTER_WIDTH - 1);

			std::cout << "Random DrawBresehamLine \n";
			std::cout << "(" << _x0 << ", " << _x1<< ") to " << "(" << _y0 << ", " << _y1 << ")\n" ;
			DrawBresehamLine(_x0, _x1, _y0, _y1, BackBuffer, 0xFFFF0000);

			DrawBresehamLine(0, 100, 499, 400, BackBuffer, 0xFFFF0000);
			DrawMidpointLine(0, 110, 499, 410, BackBuffer, 0xFF00FF00);
			DrawParametricLine(0, 120, 499, 420, BackBuffer, 0xFFFF00FF, 0xFF00FFFF);
			keyPressed = true;
		}

		if ( GetAsyncKeyState(VKNUM_2) && !keyPressed ) {
			ClearBuffer(BackBuffer);
			int _x0 = RandInRange(1, RASTER_WIDTH - 1);
			int _x1 = RandInRange(1, RASTER_WIDTH - 1);
			int _y0 = RandInRange(1, RASTER_WIDTH - 1);
			int _y1 = RandInRange(1, RASTER_WIDTH - 1);

			std::cout << "Random DrawMidpointLine \n";
			std::cout << "(" << _x0 << ", " << _x1 << ") to " << "(" << _y0 << ", " << _y1 << ")\n";

			DrawMidpointLine(_x0, _x1, _y0, _y1, BackBuffer, 0xFF00FF00);

			DrawBresehamLine(0, 100, 499, 400, BackBuffer, 0xFFFF0000);
			DrawMidpointLine(0, 110, 499, 410, BackBuffer, 0xFF00FF00);
			DrawParametricLine(0, 120, 499, 420, BackBuffer, 0xFFFF00FF, 0xFF00FFFF);
			keyPressed = true;
		}

		if ( GetAsyncKeyState(VKNUM_3) && !keyPressed ) {
			ClearBuffer(BackBuffer);
			int _x0 = RandInRange(1, RASTER_WIDTH - 1);
			int _x1 = RandInRange(1, RASTER_WIDTH - 1);
			int _y0 = RandInRange(1, RASTER_WIDTH - 1);
			int _y1 = RandInRange(1, RASTER_WIDTH - 1);

			std::cout << "Random DrawParametricLine \n";
			std::cout << "(" << _x0 << ", " << _x1 << ") to " << "(" << _y0 << ", " << _y1 << ")\n";

			DrawParametricLine(_x0, _x1, _y0, _y1, BackBuffer, 0xFFFF00FF, 0xFF00FFFF);

			DrawBresehamLine(0, 100, 499, 400, BackBuffer, 0xFFFF0000);
			DrawMidpointLine(0, 110, 499, 410, BackBuffer, 0xFF00FF00);
			DrawParametricLine(0, 120, 499, 420, BackBuffer, 0xFFFF00FF, 0xFF00FFFF);
			keyPressed = true;
		}

		if ( GetAsyncKeyState(VKNUM_4) && !keyPressed ) {
			ClearBuffer(BackBuffer);
			for ( int i = 0; i < 2500; i++ ) {
				DrawPoint(RandInRange(0, RASTER_WIDTH - 1), RandInRange(0, RASTER_HEIGHT - 1), BackBuffer, 0xFFFFFFFF);
			}

			DrawBresehamLine(0, 100, 499, 400, BackBuffer, 0xFFFF0000);
			DrawMidpointLine(0, 110, 499, 410, BackBuffer, 0xFF00FF00);
			DrawParametricLine(0, 120, 499, 420, BackBuffer, 0xFFFF00FF, 0xFF00FFFF);
			keyPressed = true;
		}

		if ( !GetAsyncKeyState(VKNUM_1) && !GetAsyncKeyState(VKNUM_2) && !GetAsyncKeyState(VKNUM_3) && !GetAsyncKeyState(VKNUM_4) ) {
			keyPressed = false;
		}
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
	//assert(_x <= RASTER_HEIGHT&&_y <= RASTER_HEIGHT);
	return _y*_width + _x;
}

int RandInRange(int _min, int _max) {
	return _min + (rand() % (int)(_max - _min + 1));
}

int Lerp_(unsigned int _A, unsigned int _B, float _ratio) {
	return (int)((((float)_B - (float)_A) * _ratio) + (float)_A);
}

unsigned int ColorLerp(unsigned int _A, unsigned int _B, float _ratio) {
	unsigned int startA = (_A & 0xFF000000) >> 24;
	unsigned int startR = (_A & 0x00FF0000) >> 16;
	unsigned int startG = (_A & 0x0000FF00) >> 8;
	unsigned int startB = (_A & 0x000000FF);

	unsigned int endA = (_B & 0xFF000000) >> 24;
	unsigned int endR = (_B & 0x00FF0000) >> 16;
	unsigned int endG = (_B & 0x0000FF00) >> 8;
	unsigned int endB = (_B & 0x000000FF);

	unsigned int newA = (unsigned int)Lerp_(startA, endA, _ratio) << 24;
	unsigned int newR = ((unsigned int)Lerp_(startR, endR, _ratio)) << 16;
	unsigned int newG = ((unsigned int)Lerp_(startG, endG, _ratio)) << 8;
	unsigned int newB = (unsigned int)Lerp_(startB, endB, _ratio);

	return newA | newR | newG| newB;
}

void DrawBresehamLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color) {
	bool steep = abs(_y1 - _y0) > abs(_x1 - _x0);
	if ( steep ) {
		std::swap(_x0, _y0);
		std::swap(_x1, _y1);
	}
	if ( _x0 > _x1 ) {
		std::swap(_x0, _x1);
		std::swap(_y0, _y1);
	}
	int deltaX = _x1 - _x0, deltaY = abs(_y1 - _y0);
	int error = deltaX / 2;
	int yStep;
	int y = _y0;
	if ( _y0 < _y1 ) yStep = 1;
	else  yStep = -1;

	for ( int x = _x0; x < _x1; x++ ) {
		if ( steep ) {
			if ( x == _x0 || x == _x1 - 1 ) DrawPoint(y, x, _buffer, 0xFFFF00);
			else DrawPoint(y, x, _buffer, _color);
		} else {
			if ( x == _x0 || x == _x1 - 1 ) DrawPoint(x, y, _buffer, 0xFFFF00);
			else DrawPoint(x, y, _buffer, _color);
		}
		error -= deltaY;
		if ( error < 0 ) {
			y += yStep;
			error += deltaX;
		}
	}

}

void DrawMidpointLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color) {
	if ( _x1 < _x0 ) {
		std::swap(_x0, _x1);
		std::swap(_y0, _y1);
	}
	int a = _y0 - _y1, b = _x1 - _x0;
	float m;
	if ( b == 0 ) m = (float)a * -100.0f;
	else m = (float)a / (_x0 - _x1);
	int x = _x0, y = _y0;
	DrawPoint(x, y, _buffer, 0xFFFF00);

	if ( m >= 0 && m <= 1 ) {
		int d = 2 * a + b;
		int d1 = 2 * a;
		int d2 = 2 * (a + b);
		while ( x < _x1 ) {
			if ( d <= 0 ) {
				x++;
				y++;
				d += d2;
			} else {
				x++;
				d += d1;
			}
			if ( x == _x1 - 1 ) DrawPoint(x, y, _buffer, 0xFFFF00);
			else DrawPoint(x, y, _buffer, _color);
		}
	} else if ( m <= 0 && m >= -1 ) {
		int d = 2 * a - b;
		int d1 = 2 * a - 2 * b;
		int d2 = 2 * a;
		while ( x < _x1 ) {
			if ( d > 0 ) {
				x++;
				y--; 
				d += d1; 
			} else { 
				x++; 
				d += d2;
			}
			if ( x == _x1 - 1 ) DrawPoint(x, y, _buffer, 0xFFFF00);
			else DrawPoint(x, y, _buffer, _color);
		}
	} else if ( m > 1 ) {
		int d = a + 2 * b;
		int d1 = 2 * (a + b);
		int d2 = 2 * b;
		while ( y < _y1 ) {
			if ( d > 0 ) { 
				x++;
				y++;
				d += d1;
			} else {
				y++;
				d += d2; 
			}
			if ( y == _y1 - 1 ) DrawPoint(x, y, _buffer, 0xFFFF00);
			else DrawPoint(x, y, _buffer, _color);
		}
	} else {
		int d = a - 2 * b;
		int d1 = -2 * b;
		int d2 = 2 * (a - b);
		while ( y > _y1 ) {
			if ( d <= 0 ) {
				x++;
				y--;
				d += d2;
			} else {
				y--;
				d += d1;
			}
			if ( y == 0 ) DrawPoint(x, y, _buffer, 0xFFFF00);
			else DrawPoint(x, y, _buffer, _color);
		}
	}
}


void DrawParametricLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _StartColor, unsigned int _EndColor) {
	int deltaX = abs(_x1 - _x0);
	int deltaY = abs(_y1 - _y0);
	int curMax = max(deltaX, deltaY);

	for ( int i = 0; i < curMax; i++ ) {
		float ratio = i / (float)curMax;
		int currX = Lerp_(_x0, _x1, ratio);
		int currY = Lerp_(_y0, _y1, ratio);
		if ( i == 0 || i == curMax - 1 ) {
			DrawPoint(currX, currY, _buffer, 0xFFFF00);
		} else {
			DrawPoint(currX, currY, _buffer, (unsigned int)ColorLerp(_StartColor, _EndColor, ratio));
		}
	}

	//for ( int currX = _x0; currX < _x1; currX++ ) {
	//	float ratio = ((float)currX - (float)_x0) / (float)deltaX;
	//	int currY = Lerp_(_y0, _y1, ratio);
	//	DrawPoint(currX, (int)floor(currY + 0.5f), _buffer, (unsigned int)ColorLerp(_StartColor, _EndColor, ratio));
	//}
}

