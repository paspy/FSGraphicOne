#pragma once
#include <cmath>
#include <Windows.h>
#include <iostream>
#include <utility>
#include <cassert>
#include <ctime>
#include <cstdio>

// marcos
#define RASTER_WIDTH 500
#define RASTER_HEIGHT 500
#define NUM_PIXELS (RASTER_WIDTH*RASTER_HEIGHT)
#define SCREEN_RATIO (RASTER_WIDTH/RASTER_HEIGHT)

#define EPSILON 0.0001
#define PI 3.1415926

#define VKNUM_1 0x31
#define VKNUM_2 0x32
#define VKNUM_3 0x33
#define VKNUM_4 0x34

// shaders

// VERTEX structure
typedef struct Vertex4 {
	union {
		// x y z w
		float xyzw[4];

		struct {
			float x, y, z, w;
		};
	};
	unsigned int color;
}*Vertex4_ptr;

typedef struct Pixel2D {
	int x, y;
}*Pixel2D_ptr;

typedef unsigned int Pixel;

typedef union Matrix4x4 {
	float e[16];

	struct {
		float _e11, _e12, _e13, _e14;
		float _e21, _e22, _e23, _e24;
		float _e31, _e32, _e33, _e34;
		float _e41, _e42, _e43, _e44;
	};
}*Matrix4x4_ptr;


inline float DegreesToradians(float _degree) {
	return (float)(_degree * PI / 180.0f);
}

// The active vertex shader. Modifies an incoming vertex. Pre-Rasterization. 
void(*VertexShader)(Vertex4&) = 0;
// The active pixel shader. Modifies an outgoing pixel. Post-Rasterization.
void(*PixelShader)(Pixel&) = 0;

// All Shader Variables (Always Pre-fixed by ¡°SV_¡±)
Matrix4x4 SV_WorldMatrix;


// function prototype
void ClearBuffer(unsigned int* _srcBuffer);
int Convert2Dto1D(const unsigned int _x, const unsigned int _y, const unsigned int _width);
int RandInRange(int _min, int _max);
unsigned int ColorLerp(unsigned int _A, unsigned int _B, float _ratio);
void DrawPoint(const unsigned int _x, const unsigned int _y, unsigned int *_buffer, unsigned int _color);
void DrawBresehamLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color);
void DrawMidpointLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color);
void DrawParametricLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _StartColor, unsigned int _EndColor);
void DrawLineUsingShader(const Vertex4 &_start, const Vertex4 &_end, unsigned int *_buffer);
void MultiplyVertexByMatrix(Vertex4 &_vertex4, Matrix4x4 _worldMatrix);
unsigned int LerpTri(unsigned int _A, unsigned int _B, unsigned int _C, float _ratioA, float _ratioB, float _ratioC);
unsigned int ColorLerpTriangle(unsigned int _A, unsigned int _B, unsigned int _C, float _ratioA, float _ratioB, float _ratioC);
Matrix4x4 MatrixRotation_Z(float _degree);
Pixel2D CartesianToScreen(Vertex4 _v4);

// Applys the current world matrix to all
void VS_World(Vertex4 &multiplyMe) {
	MultiplyVertexByMatrix(multiplyMe, SV_WorldMatrix);
}

// Basic pixel shader returns the color white
void PS_White(Pixel &makeWhite) {
	makeWhite = 0xFFFFFFFF;
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
	assert(_x <= RASTER_HEIGHT&&_y <= RASTER_HEIGHT);
	return _y*_width + _x;
}

int RandInRange(int _min, int _max) {
	return _min + (rand() % (int)(_max - _min + 1));
}

unsigned int Lerp_(unsigned int _A, unsigned int _B, float _ratio) {
	return (unsigned int)((((float)_B - (float)_A) * _ratio) + (float)_A);
}


unsigned int ColorLerpTriangle(unsigned int _A, unsigned int _B, unsigned int _C, float _ratioA, float _ratioB, float _ratioC) {
	unsigned int A1 = (_A & 0xFF000000) >> 24;
	unsigned int R1 = (_A & 0x00FF0000) >> 16;
	unsigned int G1 = (_A & 0x0000FF00) >> 8;
	unsigned int B1 = (_A & 0x000000FF);

	unsigned int A2 = (_B & 0xFF000000) >> 24;
	unsigned int R2 = (_B & 0x00FF0000) >> 16;
	unsigned int G2 = (_B & 0x0000FF00) >> 8;
	unsigned int B2 = (_B & 0x000000FF);

	unsigned int A3 = (_C & 0xFF000000) >> 24;
	unsigned int R3 = (_C & 0x00FF0000) >> 16;
	unsigned int G3 = (_C & 0x0000FF00) >> 8;
	unsigned int B3 = (_C & 0x000000FF);

	unsigned int newA = LerpTri(A1, A2, A3, _ratioA, _ratioB, _ratioC) << 24;
	unsigned int newR = LerpTri(R1, R2, R3, _ratioA, _ratioB, _ratioC) << 16;
	unsigned int newG = LerpTri(G1, G2, G3, _ratioA, _ratioB, _ratioC) << 8;
	unsigned int newB = LerpTri(B1, B2, B3, _ratioA, _ratioB, _ratioC);

	return newA | newR | newG | newB;

}

unsigned int LerpTri(unsigned int _A, unsigned int _B, unsigned int _C, float _ratioA, float _ratioB, float _ratioC) {
	return (unsigned int)(((float)_A * _ratioA) + ((float)_B * _ratioB) + ((float)_C * _ratioC));
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

	unsigned int newA = Lerp_(startA, endA, _ratio) << 24;
	unsigned int newR = Lerp_(startR, endR, _ratio) << 16;
	unsigned int newG = Lerp_(startG, endG, _ratio) << 8;
	unsigned int newB = Lerp_(startB, endB, _ratio);

	return newA | newR | newG | newB;
}

void DrawBresehamLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color) {
	if ( abs(_y1 - _y0) > abs(_x1 - _x0) ) {
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
		if ( abs(_y1 - _y0) > abs(_x1 - _x0) ) {
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
	int m = max(deltaX, deltaY);

	for ( int i = 0; i < m; i++ ) {
		float ratio = i / (float)m;
		int x = Lerp_(_x0, _x1, ratio);
		int y = Lerp_(_y0, _y1, ratio);

		Pixel copyColor = (unsigned int)ColorLerp(_StartColor, _EndColor, ratio); // Just like a Vertex, copy original. 
		if ( PixelShader ) PixelShader(copyColor); // Modify copy. 

		DrawPoint(x, y, _buffer, copyColor);
		//if (i == 0 || i == m - 1) {
		//	DrawPoint(x, y, _buffer, 0xFFFF00);
		//} else {
		//	DrawPoint(x, y, _buffer, (unsigned int)ColorLerp(_StartColor, _EndColor, ratio));
		//}
	}
}

void MultiplyVertexByMatrix(Vertex4 &_v4, Matrix4x4 _m4) {
	float x, y, z, w;
	x = _v4.x * _m4._e11 + _v4.y * _m4._e21 + _v4.z * _m4._e31 + _v4.w * _m4._e41;
	y = _v4.x * _m4._e12 + _v4.y * _m4._e22 + _v4.z * _m4._e32 + _v4.w * _m4._e42;
	z = _v4.x * _m4._e13 + _v4.y * _m4._e23 + _v4.z * _m4._e33 + _v4.w * _m4._e43;
	w = _v4.x * _m4._e14 + _v4.y * _m4._e24 + _v4.z * _m4._e34 + _v4.w * _m4._e44;
	_v4.x = x;
	_v4.y = y;
	_v4.z = z;
	_v4.w = w;
}

Matrix4x4 MatrixRotation_Z(float _degree) {
	float rad = DegreesToradians(_degree);
	Matrix4x4 m = { cosf(rad),	-sinf(rad),	0,	0,
		sinf(rad),	cosf(rad),	0,	0,
		0,			0,			1,	0,
		0,			0,			0,	1 };
	return m;
}

void DrawLineUsingShader(const Vertex4 &_start, const Vertex4 &_end, unsigned int *_buffer) {
	// Copy input data and send through shaders
	Vertex4 copy_start = _start;
	Vertex4 copy_end = _end;
	// Use vertex shader to modify incoming copies only.
	if ( VertexShader ) {
		VertexShader(copy_start);
		VertexShader(copy_end);
	}
	// original plotting variables adapted to use new cartesian data
	Pixel2D screen_start = CartesianToScreen(copy_start);
	Pixel2D screen_end = CartesianToScreen(copy_end);
	// Standard line drawing code follows using integer coordinates...

	DrawParametricLine(screen_start.x, screen_start.y, screen_end.x, screen_end.y, _buffer, _start.color, _end.color);


}

Pixel2D CartesianToScreen(Vertex4 _v4) {
	Pixel2D screenPos;

	screenPos.x = (int)((_v4.x + 1.0f) *0.5f * RASTER_WIDTH);
	screenPos.y = (int)((1.0f - _v4.y) *0.5f * RASTER_HEIGHT);

	return screenPos;
}

Vertex4 ScreenToCartesian(Pixel2D _p2) {
	Vertex4 realPos;
	realPos.x = ((float)_p2.x / RASTER_WIDTH *2.0f - 1.0f);
	realPos.y = -((float)_p2.y / RASTER_HEIGHT *2.0f - 1.0f);

	return realPos;
}

//(Y1 ¨C Y2)x + (X2 ¨C X1)y + X1Y2 ¨C Y1X2 = 0

float ImplicitLineEquation(Vertex4 _px, Vertex4 _p1, Vertex4 _p2) {
	float a, b, c, d;
	a = _p1.y - _p2.y;
	b = _p2.x - _p1.x;
	c = _p1.x * _p2.y - _p1.y * _p2.x;
	d = (a * _px.x + b * _px.y + c) / sqrtf(a*a + b*b);
	return d;
}

Vertex4 FindBarycentricPoint(Vertex4 _P, const Vertex4 _triangle[3]) {
	float beta, gamma, alpha, b, y, a;
	beta = ImplicitLineEquation(_triangle[1], _triangle[0], _triangle[2]);
	gamma = ImplicitLineEquation(_triangle[2], _triangle[1], _triangle[0]);
	alpha = ImplicitLineEquation(_triangle[0], _triangle[2], _triangle[1]);
	b = ImplicitLineEquation(_P, _triangle[0], _triangle[2]);
	y = ImplicitLineEquation(_P, _triangle[1], _triangle[0]);
	a = ImplicitLineEquation(_P, _triangle[2], _triangle[1]);
	Vertex4 P;
	P.x = b / beta;
	P.y = y / gamma;
	P.z = a / alpha;
	return P;
}

void BetterBruteTriangle(const Vertex4 _triangle[3], unsigned int *_buffer, unsigned int _color) {

	Vertex4 copy_vert[3] = { _triangle[0], _triangle[1], _triangle[2] };

	if (VertexShader) {
		VertexShader(copy_vert[0]);
		VertexShader(copy_vert[1]);
		VertexShader(copy_vert[2]);
	}

	Pixel2D a = CartesianToScreen(copy_vert[0]);
	Pixel2D b = CartesianToScreen(copy_vert[1]);
	Pixel2D c = CartesianToScreen(copy_vert[2]);

	int startX = min(a.x, min(b.x, c.x));
	int startY = min(a.y, min(b.y, c.y));
	int endX   = max(a.x, max(b.x, c.x));
	int endY   = max(a.y, max(b.y, c.y));

	for (int curY = startY; curY < endY; curY++) {
		for (int curX = startX; curX < endX; curX++) {
			Pixel2D curScreenPos;
			curScreenPos.x = curX;
			curScreenPos.y = curY;

			Vertex4 curPoint = ScreenToCartesian(curScreenPos);
			Vertex4 bya = FindBarycentricPoint(curPoint, copy_vert);
			if ((bya.x >= 0 && bya.x <= 1) &&
				(bya.y >= 0 && bya.y <= 1) &&
				(bya.z >= 0 && bya.z <= 1))
			{
				if (bya.x >= 0.99f) 					{
					int b = 0; b++;
				}
				unsigned int blendColor = ColorLerpTriangle(_triangle[0].color, _triangle[1].color, _triangle[2].color, bya.x, bya.y, bya.z);
				DrawPoint(curX, curY, _buffer, blendColor);
			}
		}
	}
}