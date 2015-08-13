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

			SV_WorldMatrix = MatrixRotation_Y(degree);
			PixelShader = PS_White;

			// draw grid
			SV_WorldMatrix = MatrixRotation_X(0.0f);
			PixelShader = PS_White;

			for ( int i = 0; i < 11; i++ ) {
				DrawLineUsingShader(Grid[0 + i], Grid[11 + i], BackBuffer);
				DrawLineUsingShader(Grid[22 + i], Grid[33 + i], BackBuffer);
			}


			
			// draw cube
			SV_WorldMatrix = MultiplyMatrixByMatrix(MatrixTranslation(0, 0.25f, 0), MatrixRotation_Y(degree));
			PixelShader = PS_Green;

			for ( int i = 0; i < 12; i++ ) {
				//DrawLineUsingShader(Cube2[i].a, Cube2[i].b, BackBuffer);
				//DrawLineUsingShader(Cube2[i].b, Cube2[i].c, BackBuffer);
				//DrawLineUsingShader(Cube2[i].c, Cube2[i].a, BackBuffer);
				BetterBruteTriangle(Cube2[i], BackBuffer, 0xFFFF0000);
			}

			if (degree >= 360.0f) degree = 0;
			frameTime = 0;
		}

	}

	RS_Shutdown();

	return 0;
}

