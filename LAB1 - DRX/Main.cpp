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
	ClearZBuffer(ZBuffer);
	XTime xTime;

	//apply shaders
	VertexShader = VS_World;
	PixelShader = PS_White;
	float degree = 0;

	double frameTime = 0;

	// View Matrix
	Matrix4x4 viewMat = SetIdentity();
	viewMat = MultiplyMatrixByMatrix(MatrixTranslation(0, 0, -1.2f), MatrixRotation_X(-30.0f));
	viewMat = Matrix_Inverse(viewMat);

	// Projection Matrix
	Matrix4x4 projMat = SetIdentity();
	projMat = CreateProjectMatrix(0.1f, 10.0f, 90.0f, 1.0f);

	bool keyPressed = false;
	int currentSelection = 0;

	while (RS_Update(BackBuffer, NUM_PIXELS)) {


		xTime.Signal();
		double deltaTime = xTime.Delta();
		frameTime += max(deltaTime, 0.0);
		
		SV_ProjMatrix = projMat;
		SV_ViewMatrix = viewMat;

		if ( frameTime > 1.0f / 60.0f ) {
			ClearBuffer(BackBuffer);
			ClearZBuffer(ZBuffer);

			// rotation here
			degree += 1.0f;

			// draw grid
			SV_WorldMatrix = MatrixRotation_X(0.0f);
			PixelShader = PS_White;

			for ( int i = 0; i < 11; i++ ) {
				DrawLineUsingShader(Grid[0 + i], Grid[11 + i], BackBuffer);
				DrawLineUsingShader(Grid[22 + i], Grid[33 + i], BackBuffer);
			}

			switch ( currentSelection ) {
			case 0: {
				// draw a tri
				SV_WorldMatrix = MultiplyMatrixByMatrix(MatrixTranslation(0, 0.25f, 0), MatrixRotation_Y(degree));
				PixelShader = PS_Green;
				BetterBruteTriangle(Cube2[4], BackBuffer, 0xFFFF0000, ZBuffer);
				break;
			}
			case 1: {
				// draw cube
				SV_WorldMatrix = MultiplyMatrixByMatrix(MatrixTranslation(0, 0.25f, 0), MatrixRotation_Y(degree));
				PixelShader = PS_Green;

				for ( int i = 0; i < 12; i++ ) {
					//DrawLineUsingShader(Cube2[i].a, Cube2[i].b, BackBuffer);
					//DrawLineUsingShader(Cube2[i].b, Cube2[i].c, BackBuffer);
					//DrawLineUsingShader(Cube2[i].c, Cube2[i].a, BackBuffer);
					BetterBruteTriangle(Cube2[i], BackBuffer, 0xFFFF0000, ZBuffer);
				}
				break;

			}
			case 2: {
				break;
			}
			default:
				break;
			}



			if (degree >= 360.0f) degree = 0;
			frameTime = 0;
		}

		if ( GetAsyncKeyState(VKNUM_1) && !keyPressed ) {
			currentSelection = 0;
			keyPressed = true;
		}

		if ( GetAsyncKeyState(VKNUM_2) && !keyPressed ) {
			currentSelection = 1;
			keyPressed = true;
		}

		if ( GetAsyncKeyState(VKNUM_3) && !keyPressed ) {
			currentSelection = 2;
			keyPressed = true;
		}

		if ( GetAsyncKeyState(VKNUM_4) && !keyPressed ) {
			currentSelection = 3;
			keyPressed = true;
		}

		if ( !GetAsyncKeyState(VKNUM_1) && !GetAsyncKeyState(VKNUM_2) && !GetAsyncKeyState(VKNUM_3) && !GetAsyncKeyState(VKNUM_4) ) {
			keyPressed = false;
		}
	}

	RS_Shutdown();

	return 0;
}

