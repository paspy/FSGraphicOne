#include <ctime>
#include <iostream>
#include <Windows.h>
#include <cassert>
#include "RasterSurface.h"
#include "Chen_2DGraphicTools.h"


unsigned int BackBuffer[NUM_PIXELS];

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

