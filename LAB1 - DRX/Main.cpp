#include <iostream>
#include <ctime>
#include <vector>
#include "RasterSurface.h"
#include "XTime.h"
#include "tiles_12.h"	// input tile file
#include "fire_02.h"	// input particle file
#include "fire_01.h"	// input particle file


// grass coord = 288*128 to 319*159

#define RASTER_WIDTH 500
#define RASTER_HEIGHT 500
#define NUM_PIXELS (RASTER_WIDTH*RASTER_HEIGHT)

struct Rect {
	Rect() {}
	Rect(unsigned int _left, unsigned int _top, unsigned int _right, unsigned int _bottom) : left(_left), top(_top), right(_right), bottom(_bottom) {}
 	unsigned int left;
	unsigned int top;
	unsigned int right;
	unsigned int bottom;
};

struct Point {
	Point() {}
	Point(unsigned int _x, unsigned int _y) : x(_x), y(_y) {}
	unsigned int x, y;
};

struct Frame {
	unsigned int BackBuffer[NUM_PIXELS];
};

Frame frames[64];


// function prototype
void ClearBuffer(unsigned int* _srcBuffer);
int Convert2Dto1D(const unsigned int _x, const unsigned int _y, const unsigned int _width);
int RandInRange(int _min, int _max);
unsigned int LerpARGB(unsigned int _A, unsigned int _B, float _ratio);
void BlockImageTransfer(const unsigned int* _srcImgArr, unsigned int* _desImgArr,
						const unsigned int _srcImgheight, const unsigned int _srcImgWidth,
						const unsigned int _desImgheight, const unsigned int _desImgWidth,
						const Rect _rect,
						const unsigned int _copyToX, const unsigned int _copyToY);

void PlayAnimation(Rect* _animeArr, unsigned int _curFrame);
void BlendingLayers(unsigned int * _BackBuffer, const unsigned int * _layer0, const unsigned int * _layer1, const unsigned int * _layer2);


int main() {
	srand(unsigned int(time(nullptr)));

	RS_Initialize(RASTER_WIDTH, RASTER_HEIGHT);


	for (int i = 0; i < 64; i++) {
		ClearBuffer(frames[i].BackBuffer);
	}

	Rect backgroundTile(288, 128, 288 + 31, 128 + 31);
	Rect tree(320, 0, 320 + 64, 0 + 96);
	Rect cellAnimArr[64];

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Rect cell(i * 128, j * 128, i * 128 + 128, j * 128 + 128);
			cellAnimArr[i * 8 + j] = cell;
		}
	}

	XTime xTime;
	
	Point rndPoints[10];

	for (int i = 0; i < 10; i++) {
		rndPoints[i].x = RandInRange(0, RASTER_WIDTH - 64);
		rndPoints[i].y = RandInRange(0, RASTER_HEIGHT - 96);
	}

	//draw Background
	for (int idx = 0; idx < 64; idx++) {
		for (int i = 0; i <= RASTER_WIDTH; i += 32) {
			for (int j = 0; j <= RASTER_HEIGHT; j += 32) {
				BlockImageTransfer(tiles_12_pixels, frames[idx].BackBuffer, tiles_12_height, tiles_12_width, RASTER_HEIGHT, RASTER_WIDTH, backgroundTile, i, j);
			}
		}
	}
	
	// blend ramdon trees
	for (int idx = 0; idx < 64; idx++) {
		for (int i = 0; i < 10; i++) {
			BlockImageTransfer(tiles_12_pixels, frames[idx].BackBuffer, tiles_12_height, tiles_12_width, RASTER_HEIGHT, RASTER_WIDTH, tree, rndPoints[i].x, rndPoints[i].y);
		}
	}

	// bake animation to each frame
	for (int idx = 0; idx < 64; idx++) {
		PlayAnimation(cellAnimArr, idx);
	}
	
	double frameTime = 0;
	int currentFrame = 0;

	while (RS_Update(frames[currentFrame].BackBuffer, NUM_PIXELS)) {
		std::cout << currentFrame << std::endl;
		do {
			Sleep(1);
			xTime.Signal();
			double deltaTime = xTime.Delta();
			frameTime += max(deltaTime, 0.0);
			//std::cout << frameTime << std::endl;
		} while (frameTime < 1.0 / 60.0);
		frameTime -= 1.0 / 60.0;
		//currentFrame++;
		if (++currentFrame >= 64) currentFrame = 0;
	}

	//do {

		//do {
		//	Sleep(25);
		//	xTime.Signal();
		//	double deltaTime = xTime.Delta();
		//	frameTime += max(deltaTime, 0.0);
		//	//std::cout << frameTime << std::endl;
		//} while (frameTime < 1.0 / 60.0);
		//frameTime -= 1.0 / 60.0;

	//	Sleep(50);

	//	currentFrame++;
	//	std::cout << currentFrame << std::endl;
	//	if (currentFrame >= 64) currentFrame = -1;


	//} while (  );

	RS_Shutdown();

	return 0;
}


void PlayAnimation(Rect* _animeArr, unsigned int _curFrame) {
	BlockImageTransfer(fire_01_pixels, frames[_curFrame].BackBuffer, fire_01_height, fire_01_width, RASTER_HEIGHT, RASTER_WIDTH, _animeArr[_curFrame], RASTER_HEIGHT >> 1, RASTER_WIDTH >> 1);
}

void ClearBuffer(unsigned int* _srcBuffer) {
	for ( int i = 0; i < NUM_PIXELS; i++ ) {
		_srcBuffer[i] = 0xFFFF0000;
	}
}

int Convert2Dto1D(const unsigned int _x, const unsigned int _y, const unsigned int _width) {
	return _y*_width + _x;
}

void BlockImageTransfer(const unsigned int* _srcImgArr, unsigned int* _desImgArr,
						const unsigned int _srcImgheight, const unsigned int _srcImgWidth,
						const unsigned int _desImgheight, const unsigned int _desImgWidth,
						const Rect _rect,
						const unsigned int _copyToX, const unsigned int _copyToY) {

	for ( unsigned int i = _rect.left; i <= _rect.right; i++ ) {

		for ( unsigned int j = _rect.top; j <= _rect.bottom; j++ ) {

			unsigned int inColor = _srcImgArr[Convert2Dto1D(i, j, _srcImgWidth)];	// BGRA to ARGB
			unsigned int ix = inColor & 0x000000FF; ix <<= 24;
			unsigned int ir = inColor & 0x0000FF00; ir <<= 8;
			unsigned int ig = inColor & 0x00FF0000; ig >>= 8;
			unsigned int ib = inColor & 0xFF000000; ib >>= 24;
			
			float ratio = 1 - (((float)(ix >> 24)) / 255.0f);

			unsigned int orginColor = _desImgArr[Convert2Dto1D(i + _copyToX - _rect.left, j + _copyToY - _rect.top, _desImgWidth)];
			unsigned int orginX = orginColor & 0xFF000000;
			unsigned int orginR = orginColor & 0x00FF0000;
			unsigned int orginG = orginColor & 0x0000FF00;
			unsigned int orginB = orginColor & 0x000000FF;

			unsigned int newX = LerpARGB(ix >> 24,	orginX >> 24,	ratio); newX <<= 24;
			unsigned int newR = LerpARGB(ir >> 16,	orginR >> 16,	ratio); newR <<= 16;
			unsigned int newG = LerpARGB(ig >> 8,	orginG >> 8,	ratio); newG <<= 8;
			unsigned int newB = LerpARGB(ib,		orginB,			ratio);

			unsigned int outColor = newX | newR | newG | newB;

			unsigned int outColor_break = ix | ir | ig | ib;

			_desImgArr[Convert2Dto1D(i + _copyToX - _rect.left, j + _copyToY - _rect.top, _desImgWidth)] = outColor;

		}
	}

}

unsigned int LerpARGB(unsigned int _A, unsigned int _B, float _ratio) {
	return (unsigned int)((((int)_B - (int)_A) * _ratio) + _A);
	//return ((_B - _A) * _ratio) + _A;
}

int RandInRange(int _min, int _max) {
	return _min + (rand() % (int)(_max - _min + 1));
}