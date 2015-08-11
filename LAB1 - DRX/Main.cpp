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

	// grid
	
	const Vertex4 Grid[44] = {
		{ 0.5f	, 0.f	, -0.50f	, 1.f	, 0xFFFFFFFF },	// 0
		{ 0.5f	, 0.f	, -0.40f	, 1.f	, 0xFFFFFFFF },
		{ 0.5f	, 0.f	, -0.30f	, 1.f	, 0xFFFFFFFF },
		{ 0.5f	, 0.f	, -0.20f	, 1.f	, 0xFFFFFFFF },
		{ 0.5f	, 0.f	, -0.10f	, 1.f	, 0xFFFFFFFF },
		{ 0.5f	, 0.f	,  0.00f	, 1.f	, 0xFFFFFFFF },
		{ 0.5f	, 0.f	,  0.10f	, 1.f	, 0xFFFFFFFF },
		{ 0.5f	, 0.f	,  0.20f	, 1.f	, 0xFFFFFFFF },
		{ 0.5f	, 0.f	,  0.30f	, 1.f	, 0xFFFFFFFF },
		{ 0.5f	, 0.f	,  0.40f	, 1.f	, 0xFFFFFFFF },
		{ 0.5f	, 0.f	,  0.50f	, 1.f	, 0xFFFFFFFF },

		{ -0.5f	, 0.f	, -0.50f	, 1.f	, 0xFFFFFFFF },	// 11
		{ -0.5f	, 0.f	, -0.40f	, 1.f	, 0xFFFFFFFF },
		{ -0.5f	, 0.f	, -0.30f	, 1.f	, 0xFFFFFFFF },
		{ -0.5f	, 0.f	, -0.20f	, 1.f	, 0xFFFFFFFF },
		{ -0.5f	, 0.f	, -0.10f	, 1.f	, 0xFFFFFFFF },
		{ -0.5f	, 0.f	,  0.00f	, 1.f	, 0xFFFFFFFF },
		{ -0.5f	, 0.f	,  0.10f	, 1.f	, 0xFFFFFFFF },
		{ -0.5f	, 0.f	,  0.20f	, 1.f	, 0xFFFFFFFF },
		{ -0.5f	, 0.f	,  0.30f	, 1.f	, 0xFFFFFFFF },
		{ -0.5f	, 0.f	,  0.40f	, 1.f	, 0xFFFFFFFF },
		{ -0.5f	, 0.f	,  0.50f	, 1.f	, 0xFFFFFFFF },

		{ -0.50f	, 0.f	, 0.5f	, 1.f	, 0xFFFFFFFF }, // 22
		{ -0.40f	, 0.f	, 0.5f	, 1.f	, 0xFFFFFFFF },
		{ -0.30f	, 0.f	, 0.5f	, 1.f	, 0xFFFFFFFF },
		{ -0.20f	, 0.f	, 0.5f	, 1.f	, 0xFFFFFFFF },
		{ -0.10f	, 0.f	, 0.5f	, 1.f	, 0xFFFFFFFF },
		{  0.00f	, 0.f	, 0.5f	, 1.f	, 0xFFFFFFFF },
		{  0.10f	, 0.f	, 0.5f	, 1.f	, 0xFFFFFFFF },
		{  0.20f	, 0.f	, 0.5f	, 1.f	, 0xFFFFFFFF },
		{  0.30f	, 0.f	, 0.5f	, 1.f	, 0xFFFFFFFF },
		{  0.40f	, 0.f	, 0.5f	, 1.f	, 0xFFFFFFFF },
		{  0.50f	, 0.f	, 0.5f	, 1.f	, 0xFFFFFFFF },
				
		{ -0.50f	, 0.f	, -0.5f	, 1.f	, 0xFFFFFFFF }, // 33
		{ -0.40f	, 0.f	, -0.5f	, 1.f	, 0xFFFFFFFF },
		{ -0.30f	, 0.f	, -0.5f	, 1.f	, 0xFFFFFFFF },
		{ -0.20f	, 0.f	, -0.5f	, 1.f	, 0xFFFFFFFF },
		{ -0.10f	, 0.f	, -0.5f	, 1.f	, 0xFFFFFFFF },
		{  0.00f	, 0.f	, -0.5f	, 1.f	, 0xFFFFFFFF },
		{  0.10f	, 0.f	, -0.5f	, 1.f	, 0xFFFFFFFF },
		{  0.20f	, 0.f	, -0.5f	, 1.f	, 0xFFFFFFFF },
		{  0.30f	, 0.f	, -0.5f	, 1.f	, 0xFFFFFFFF },
		{  0.40f	, 0.f	, -0.5f	, 1.f	, 0xFFFFFFFF },
		{  0.50f	, 0.f	, -0.5f	, 1.f	, 0xFFFFFFFF },
	};

	// Cube
	const Vertex4 Cube[8] = {
		{ -0.25f, 0.0f,  0.25f, 1.f, 0xFF00FF00 },	// A
		{  0.25f, 0.0f,  0.25f, 1.f, 0xFF00FF00 },	// B
		{  0.25f, 0.0f, -0.25f, 1.f, 0xFF00FF00 },	// C
		{ -0.25f, 0.0f, -0.25f, 1.f, 0xFF00FF00 },	// D
		
		{ -0.25f, 0.5f,  0.25f, 1.f, 0xFF00FF00 },	// E
		{ 0.25f,  0.5f,  0.25f, 1.f, 0xFF00FF00 },	// F
		{ 0.25f,  0.5f, -0.25f, 1.f, 0xFF00FF00 },	// G
		{ -0.25f, 0.5f, -0.25f, 1.f, 0xFF00FF00 },	// H
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

			SV_WorldMatrix = MatrixRotation_X(0.0f);

			//BetterBruteTriangle(Triangle, BackBuffer, 0xFFFF0000);

			// draw grid
			PixelShader = PS_White;

			for ( int i = 0; i < 11; i++ ) {
				DrawLineUsingShader(Grid[0 + i], Grid[11 + i], BackBuffer);
				DrawLineUsingShader(Grid[22 + i], Grid[33 + i], BackBuffer);
			}


			// draw cube 
			SV_WorldMatrix = MatrixRotation_Y(degree);

			PixelShader = PS_Green;

			DrawLineUsingShader(Cube[0], Cube[1], BackBuffer);
			DrawLineUsingShader(Cube[1], Cube[2], BackBuffer);
			DrawLineUsingShader(Cube[2], Cube[3], BackBuffer);
			DrawLineUsingShader(Cube[3], Cube[0], BackBuffer);

			DrawLineUsingShader(Cube[4], Cube[5], BackBuffer);
			DrawLineUsingShader(Cube[5], Cube[6], BackBuffer);
			DrawLineUsingShader(Cube[6], Cube[7], BackBuffer);
			DrawLineUsingShader(Cube[7], Cube[4], BackBuffer);

			DrawLineUsingShader(Cube[0], Cube[4], BackBuffer);
			DrawLineUsingShader(Cube[1], Cube[5], BackBuffer);
			DrawLineUsingShader(Cube[2], Cube[6], BackBuffer);
			DrawLineUsingShader(Cube[3], Cube[7], BackBuffer);


			//DrawLineUsingShader(Triangle[0], Triangle[1], BackBuffer);
			//DrawLineUsingShader(Triangle[1], Triangle[2], BackBuffer);
			//DrawLineUsingShader(Triangle[2], Triangle[0], BackBuffer);

			if (degree >= 360.0f) degree = 0;
			frameTime = 0;
		}

	}

	RS_Shutdown();

	return 0;
}

