#include <ctime>
#include <iostream>
#include <Windows.h>
#include <cassert>
#include "RasterSurface.h"
#include "Chen_2DGraphicTools.h"
#include "XTime.h"


unsigned int BackBuffer[NUM_PIXELS];


int main() {
	srand(unsigned int(time(nullptr)));

	RS_Initialize(RASTER_WIDTH, RASTER_HEIGHT);

	ClearBuffer(BackBuffer);

	//Triangle
	Vertex4 a, b, c;
	a.xyzw[0] = 0; a.xyzw[1] = 0.5f; a.xyzw[2] = 0; a.xyzw[3] = 0; a.color = 0xFFFF0000;
	b.xyzw[0] = 0.5f; b.xyzw[1] = 0; b.xyzw[2] = 0; b.xyzw[3] = 0; b.color = 0xFF00FF00;
	c.xyzw[0] = -0.5f; c.xyzw[1] = 0; c.xyzw[2] = 0; c.xyzw[3] = 0; c.color = 0xFF0000FF;

	XTime xTime;

	//apply shaders
	VertexShader = VS_World;
	PixelShader = PS_White;

	double frameTime = 0;
	while (RS_Update(BackBuffer, NUM_PIXELS)) {
		xTime.Signal();
		double deltaTime = xTime.Delta();
		frameTime += max(deltaTime, 0.0);

		if ( frameTime > 1.0 / 120.0 ) {
			ClearBuffer(BackBuffer);
			// rotation here
			SV_WorldMatrix = MatrixRotation_Z((float)(frameTime * 100));

			MultiplyVertexByMatrix(a, SV_WorldMatrix);
			MultiplyVertexByMatrix(b, SV_WorldMatrix);
			MultiplyVertexByMatrix(c, SV_WorldMatrix);

			DrawLineUsingShader(a, b, BackBuffer);
			DrawLineUsingShader(b, c, BackBuffer);
			DrawLineUsingShader(c, a, BackBuffer);

			frameTime = 0;
		}


/*
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
		}*/
	}

	RS_Shutdown();

	return 0;
}

