#include <iostream>
#include <ctime>
#include "RasterSurface.h"
#include "XTime.h"
#include "tiles_12.h"	// input tile file
#include "fire_02.h"	// input particle file


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

unsigned int BackBuffer[NUM_PIXELS];

// function prototype
void ClearBuffer(unsigned int* _srcBuffer);
int Convert2Dto1D(const unsigned int _x, const unsigned int _y, const unsigned int _width);
unsigned int LerpARGB(unsigned int _A, unsigned int _B, float _ratio);
void BlockImageTransfer(const unsigned int* _srcImgArr, unsigned int* _desImgArr,
						const unsigned int _srcImgheight, const unsigned int _srcImgWidth,
						const unsigned int _desImgheight, const unsigned int _desImgWidth,
						const Rect _rect,
						const unsigned int _copyToX, const unsigned int _copyToY);

void PlayAnimation(Rect* _animeArr, unsigned int _curFrame);

int main() {
	srand(unsigned int(time(nullptr)));

	RS_Initialize(RASTER_WIDTH, RASTER_HEIGHT);
	ClearBuffer(BackBuffer);

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
	
	
	int rLocX = rand() % RASTER_HEIGHT;
	int rLocY = rand() % RASTER_WIDTH;
	

	int currentFrame = 0;
	do {
		xTime.Signal();

		// Background
		for (int i = 0; i < RASTER_WIDTH; i += 31) {
			for (int j = 0; j < RASTER_HEIGHT; j += 31) {
				BlockImageTransfer(tiles_12_pixels, BackBuffer, tiles_12_height, tiles_12_width, RASTER_HEIGHT, RASTER_WIDTH, backgroundTile, i, j);
			}
		}

		// ramdon trees
		for (int i = 0; i < 10; i++) {
			int rLocX = rand() % RASTER_HEIGHT;
			int rLocY = rand() % RASTER_WIDTH;
			BlockImageTransfer(tiles_12_pixels, BackBuffer, tiles_12_height, tiles_12_width, RASTER_HEIGHT, RASTER_WIDTH, tree, rLocX, rLocY);
		}

		PlayAnimation(cellAnimArr, currentFrame);
		currentFrame++;
		if (currentFrame > 64) currentFrame = 0;
	} while ( RS_Update(BackBuffer, NUM_PIXELS) );

	RS_Shutdown();

	return 0;
}

void PlayAnimation(Rect* _animeArr, unsigned int _curFrame) {
	BlockImageTransfer(fire_02_pixels, BackBuffer, fire_02_height, fire_02_width, RASTER_HEIGHT, RASTER_WIDTH, _animeArr[_curFrame], RASTER_HEIGHT >> 1, RASTER_WIDTH >> 1);
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

	for ( unsigned int i = _rect.left; i < _rect.right; i++ ) {
		for ( unsigned int j = _rect.top; j < _rect.bottom; j++ ) {
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