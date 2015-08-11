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

	const Vertex4 Triangle[3] = {
		{ 0, 0.5f, 0, 0, 0xFFFF0000 },
		{ 0.5f, 0, 0, 0, 0xFF00FF00 },
		{ -0.5f, 0, 0, 0, 0xFF0000FF },
	};

	XTime xTime;

	//apply shaders
	VertexShader = VS_World;
	PixelShader = PS_White;
	float degree = 0;

	double frameTime = 0;
	while (RS_Update(BackBuffer, NUM_PIXELS)) {
		xTime.Signal();
		double deltaTime = xTime.Delta();
		frameTime += max(deltaTime, 0.0);

		if ( frameTime > 1.0 / 30.0 ) {
			ClearBuffer(BackBuffer);
			// rotation here
			degree += 1.0f;

			SV_WorldMatrix = MatrixRotation_Z(degree);

			BetterBruteTriangle(Triangle, BackBuffer, 0xFFFF0000);

			DrawLineUsingShader(Triangle[0], Triangle[1], BackBuffer);
			DrawLineUsingShader(Triangle[1], Triangle[2], BackBuffer);
			DrawLineUsingShader(Triangle[2], Triangle[0], BackBuffer);

			if (degree >= 360.0f) degree = 0;
			frameTime = 0;
		}

	}

	RS_Shutdown();

	return 0;
}

