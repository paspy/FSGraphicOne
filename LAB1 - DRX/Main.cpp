#include <ctime>
#include <iostream>
#include <Windows.h>
#include <cassert>
#include "RasterSurface.h"
#include "XTime.h"
#include "3DModel.h"

unsigned int BackBuffer[NUM_PIXELS];
float ZBuffer[NUM_PIXELS];

int main() {
	srand(unsigned int(time(nullptr)));

	RS_Initialize(RASTER_WIDTH, RASTER_HEIGHT);

	ClearBuffer(BackBuffer);

	XTime xTime;

	//apply shaders
	VertexShader = VS_World;
	PixelShader = PS_White;
	float degree = 0;

	double frameTime = 0;

	// View Matrix
	Matrix4x4 viewMat = SetIdentity();
	viewMat = MultiplyMatrixByMatrix(MatrixTranslation(0, 0, -1.0f), MatrixRotation_X(-18.0f));
	viewMat = Matrix_Inverse(viewMat);

	// Projection Matrix
	Matrix4x4 projMat = SetIdentity();
	projMat = CreateProjectMatrix(0.1f, 10.0f, 90.0f, 1.0f);

	while (RS_Update(BackBuffer, NUM_PIXELS)) {
		xTime.Signal();
		double deltaTime = xTime.Delta();
		frameTime += max(deltaTime, 0.0);
		
		SV_ProjMatrix = projMat;
		SV_ViewMatrix = viewMat;

		if ( frameTime > 1.0f / 60.0f ) {
			ClearBuffer(BackBuffer);
			// rotation here
			degree += 1.0f;

			SV_WorldMatrix = MatrixRotation_Z(degree);
			PixelShader = PS_White;
			BetterBruteTriangle(Triangle, BackBuffer, 0xFFFF0000);

			// draw grid
			SV_WorldMatrix = MatrixRotation_X(0.0f);
			PixelShader = PS_White;

			for ( int i = 0; i < 11; i++ ) {
				DrawLineUsingShader(Grid[0 + i], Grid[11 + i], BackBuffer);
				DrawLineUsingShader(Grid[22 + i], Grid[33 + i], BackBuffer);
			}

			//// draw cube
			//SV_WorldMatrix = MultiplyMatrixByMatrix(MatrixTranslation(0, 0.25f, 0), MatrixRotation_Y(degree));

			//PixelShader = PS_Green;

			//DrawLineUsingShader(Cube[0], Cube[1], BackBuffer);
			//DrawLineUsingShader(Cube[1], Cube[2], BackBuffer);
			//DrawLineUsingShader(Cube[2], Cube[3], BackBuffer);
			//DrawLineUsingShader(Cube[3], Cube[0], BackBuffer);

			//DrawLineUsingShader(Cube[4], Cube[5], BackBuffer);
			//DrawLineUsingShader(Cube[5], Cube[6], BackBuffer);
			//DrawLineUsingShader(Cube[6], Cube[7], BackBuffer);
			//DrawLineUsingShader(Cube[7], Cube[4], BackBuffer);
			//DrawLineUsingShader(Cube[7], Cube[4], BackBuffer);

			//DrawLineUsingShader(Cube[0], Cube[4], BackBuffer);
			//DrawLineUsingShader(Cube[1], Cube[5], BackBuffer);
			//DrawLineUsingShader(Cube[2], Cube[6], BackBuffer);
			//DrawLineUsingShader(Cube[3], Cube[7], BackBuffer);

			//DrawLineUsingShader(Cube[0], Cube[2], BackBuffer);
			//DrawLineUsingShader(Cube[0], Cube[5], BackBuffer);
			//DrawLineUsingShader(Cube[0], Cube[7], BackBuffer);
			//DrawLineUsingShader(Cube[1], Cube[6], BackBuffer);
			//DrawLineUsingShader(Cube[3], Cube[6], BackBuffer);
			//DrawLineUsingShader(Cube[4], Cube[6], BackBuffer);

			if (degree >= 360.0f) degree = 0;
			frameTime = 0;
		}

	}

	RS_Shutdown();

	return 0;
}

