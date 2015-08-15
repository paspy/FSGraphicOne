#pragma once
#include <cmath>
#include <Windows.h>
#include <iostream>
#include <ctime>
#include <cstdio>
#include "Celestial.h"

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
	union {
		float uv[2];
		struct {
			float u, v;
		};
	};
	unsigned int color;
}*Vertex4_ptr;

typedef struct Tri {
	Vertex4 a, b, c;
}*Triangle_ptr;

typedef struct Pixel2D {
	int x, y;
	float depth;
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

inline Matrix4x4 SetIdentity() {
	Matrix4x4 m = { 0 };
	m._e11 = 1.0f;
	m._e22 = 1.0f;
	m._e33 = 1.0f;
	m._e44 = 1.0f;
	return m;
}

bool IsZero(float a) {
	return (fabs(a))<EPSILON;
}

// The active vertex shader. Modifies an incoming vertex. Pre-Rasterization. 
void(*VertexShader)(Vertex4&) = 0;
// The active pixel shader. Modifies an outgoing pixel. Post-Rasterization.
void(*PixelShader)(Pixel&) = 0;

// All Shader Variables (Always Pre-fixed by ¡°SV_¡±)
Matrix4x4 SV_WorldMatrix;
Matrix4x4 SV_ViewMatrix;
Matrix4x4 SV_ProjMatrix;

// function prototype
void ClearBuffer(unsigned int* _srcBuffer);
void ClearZBuffer(float* _zBuffer);
int Convert2Dto1D(const unsigned int _x, const unsigned int _y, const unsigned int _width);
int RandInRange(int _min, int _max);
unsigned int ColorLerp(unsigned int _A, unsigned int _B, float _ratio);
void DrawPoint(const unsigned int _x, const unsigned int _y, unsigned int *_buffer, unsigned int _color, float *_zBuffer, float _depth);
//void DrawBresehamLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color);
//void DrawMidpointLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color);
void DrawParametricLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _StartColor, unsigned int _EndColor, float *_zBuffer, float zValue);
void DrawLineUsingShader(const Vertex4 &_start, const Vertex4 &_end, unsigned int *_buffer, float *_zBuffer);
void MultiplyVertexByMatrix(Vertex4 &_vertex4, Matrix4x4 _worldMatrix);
Matrix4x4 MultiplyMatrixByMatrix(Matrix4x4 m, Matrix4x4 n);
unsigned int LerpTri(unsigned int _A, unsigned int _B, unsigned int _C, float _ratioA, float _ratioB, float _ratioC);
unsigned int BGRA_To_ARGB(const unsigned int _inColor);
unsigned int ColorLerpTriangle(unsigned int _A, unsigned int _B, unsigned int _C, float _ratioA, float _ratioB, float _ratioC);
Matrix4x4 MatrixRotation_X(float _degree);
Matrix4x4 MatrixRotation_Y(float _degree);
Matrix4x4 MatrixRotation_Z(float _degree);
Pixel2D CartesianToScreen(Vertex4 _v4);

// Applys the current world matrix to all
void VS_World(Vertex4 &multiplyMe) {
	MultiplyVertexByMatrix(multiplyMe, SV_WorldMatrix);
	MultiplyVertexByMatrix(multiplyMe, SV_ViewMatrix);
	MultiplyVertexByMatrix(multiplyMe, SV_ProjMatrix);
	multiplyMe.x /= multiplyMe.w;
	multiplyMe.y /= multiplyMe.w;
	multiplyMe.z /= multiplyMe.w;
}

// Basic pixel shader returns the color white
void PS_White(Pixel &makeWhite) {
	makeWhite = 0xFFFFFFFF;
}

void PS_Green(Pixel &makeWhite) {
	makeWhite = 0xFF00FF00;
}

void PS_UVShader(Pixel &_inColor, float _u, float _v) {
	int idx = int( std::floorf(_v*celestial_height) * celestial_width + std::floorf(_u*celestial_width));
	if ( idx >= celestial_numpixels ) return;
	_inColor = BGRA_To_ARGB(celestial_pixels[idx]);

}


void ClearBuffer(unsigned int* _srcBuffer) {
	for ( int i = 0; i < NUM_PIXELS; i++ ) {
		_srcBuffer[i] = 0xFF000000;
	}
}

void ClearZBuffer(float* _zBuffer) {
	for ( int i = 0; i < NUM_PIXELS; i++ ) {
		_zBuffer[i] = 1.0f;
	}
}

void DrawPoint(const unsigned int _x, const unsigned int _y, unsigned int *_buffer, unsigned int _color, float *_zBuffer, float _depth) {

	int index = Convert2Dto1D(_x, _y, RASTER_WIDTH);

	if ( index < 0 || _depth > _zBuffer[index]) return;

	_zBuffer[index] = _depth;
	_buffer[index] = _color;
}

int Convert2Dto1D(const unsigned int _x, const unsigned int _y, const unsigned int _width) {
	if ( _x <= RASTER_HEIGHT&&_y <= RASTER_HEIGHT )	return _y*_width + _x;
	return -1;
}

int RandInRange(int _min, int _max) {
	return _min + (rand() % (int)(_max - _min + 1));
}

unsigned int Lerp_(unsigned int _A, unsigned int _B, float _ratio) {
	return (unsigned int)((((float)_B - (float)_A) * _ratio) + (float)_A);
}

unsigned int BGRA_To_ARGB(const unsigned int _inColor) {
	// BGRA to ARGB
	unsigned int ia = _inColor & 0x000000FF; ia <<= 24;
	unsigned int ir = _inColor & 0x0000FF00; ir <<= 8;
	unsigned int ig = _inColor & 0x00FF0000; ig >>= 8;
	unsigned int ib = _inColor & 0xFF000000; ib >>= 24;

	return (ia | ir | ig | ib);
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

//void DrawBresehamLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color) {
//	if ( abs(_y1 - _y0) > abs(_x1 - _x0) ) {
//		std::swap(_x0, _y0);
//		std::swap(_x1, _y1);
//	}
//	if ( _x0 > _x1 ) {
//		std::swap(_x0, _x1);
//		std::swap(_y0, _y1);
//	}
//	int deltaX = _x1 - _x0, deltaY = abs(_y1 - _y0);
//	int error = deltaX / 2;
//	int yStep;
//	int y = _y0;
//	if ( _y0 < _y1 ) yStep = 1;
//	else  yStep = -1;
//	for ( int x = _x0; x < _x1; x++ ) {
//		if ( abs(_y1 - _y0) > abs(_x1 - _x0) ) {
//			if ( x == _x0 || x == _x1 - 1 ) DrawPoint(y, x, _buffer, 0xFFFF00);
//			else DrawPoint(y, x, _buffer, _color);
//		} else {
//			if ( x == _x0 || x == _x1 - 1 ) DrawPoint(x, y, _buffer, 0xFFFF00);
//			else DrawPoint(x, y, _buffer, _color);
//		}
//		error -= deltaY;
//		if ( error < 0 ) {
//			y += yStep;
//			error += deltaX;
//		}
//	}
//
//}
//
//void DrawMidpointLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color) {
//	if ( _x1 < _x0 ) {
//		std::swap(_x0, _x1);
//		std::swap(_y0, _y1);
//	}
//	int a = _y0 - _y1, b = _x1 - _x0;
//	float m;
//	if ( b == 0 ) m = (float)a * -100.0f;
//	else m = (float)a / (_x0 - _x1);
//	int x = _x0, y = _y0;
//	DrawPoint(x, y, _buffer, 0xFFFF00);
//
//	if ( m >= 0 && m <= 1 ) {
//		int d = 2 * a + b;
//		int d1 = 2 * a;
//		int d2 = 2 * (a + b);
//		while ( x < _x1 ) {
//			if ( d <= 0 ) {
//				x++;
//				y++;
//				d += d2;
//			} else {
//				x++;
//				d += d1;
//			}
//			if ( x == _x1 - 1 ) DrawPoint(x, y, _buffer, 0xFFFF00);
//			else DrawPoint(x, y, _buffer, _color);
//		}
//	} else if ( m <= 0 && m >= -1 ) {
//		int d = 2 * a - b;
//		int d1 = 2 * a - 2 * b;
//		int d2 = 2 * a;
//		while ( x < _x1 ) {
//			if ( d > 0 ) {
//				x++;
//				y--;
//				d += d1;
//			} else {
//				x++;
//				d += d2;
//			}
//			if ( x == _x1 - 1 ) DrawPoint(x, y, _buffer, 0xFFFF00);
//			else DrawPoint(x, y, _buffer, _color);
//		}
//	} else if ( m > 1 ) {
//		int d = a + 2 * b;
//		int d1 = 2 * (a + b);
//		int d2 = 2 * b;
//		while ( y < _y1 ) {
//			if ( d > 0 ) {
//				x++;
//				y++;
//				d += d1;
//			} else {
//				y++;
//				d += d2;
//			}
//			if ( y == _y1 - 1 ) DrawPoint(x, y, _buffer, 0xFFFF00);
//			else DrawPoint(x, y, _buffer, _color);
//		}
//	} else {
//		int d = a - 2 * b;
//		int d1 = -2 * b;
//		int d2 = 2 * (a - b);
//		while ( y > _y1 ) {
//			if ( d <= 0 ) {
//				x++;
//				y--;
//				d += d2;
//			} else {
//				y--;
//				d += d1;
//			}
//			if ( y == 0 ) DrawPoint(x, y, _buffer, 0xFFFF00);
//			else DrawPoint(x, y, _buffer, _color);
//		}
//	}
//}

void DrawParametricLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer,  unsigned int _StartColor, unsigned int _EndColor, float *_zBuffer, float zValue) {
	int deltaX = abs(_x1 - _x0);
	int deltaY = abs(_y1 - _y0);
	int m = max(deltaX, deltaY);

	for ( int i = 0; i < m; i++ ) {
		float ratio = i / (float)m;
		int x = Lerp_(_x0, _x1, ratio);
		int y = Lerp_(_y0, _y1, ratio);

		Pixel copyColor = (unsigned int)ColorLerp(_StartColor, _EndColor, ratio); // Just like a Vertex, copy original. 
		if ( PixelShader ) PixelShader(copyColor); // Modify copy. 

		DrawPoint(x, y, _buffer, copyColor, _zBuffer, zValue);
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

Matrix4x4 MultiplyMatrixByMatrix(Matrix4x4 m, Matrix4x4 n) {
	Matrix4x4 r;

	r._e11 = m._e11 * n._e11 + m._e12 * n._e21 + m._e13 * n._e31 + m._e14 * n._e41;
	r._e12 = m._e11 * n._e12 + m._e12 * n._e22 + m._e13 * n._e32 + m._e14 * n._e42;
	r._e13 = m._e11 * n._e13 + m._e12 * n._e23 + m._e13 * n._e33 + m._e14 * n._e43;
	r._e14 = m._e11 * n._e14 + m._e12 * n._e24 + m._e13 * n._e34 + m._e14 * n._e44;

	r._e21 = m._e21 * n._e11 + m._e22 * n._e21 + m._e23 * n._e31 + m._e24 * n._e41;
	r._e22 = m._e21 * n._e12 + m._e22 * n._e22 + m._e23 * n._e32 + m._e24 * n._e42;
	r._e23 = m._e21 * n._e13 + m._e22 * n._e23 + m._e23 * n._e33 + m._e24 * n._e43;
	r._e24 = m._e21 * n._e14 + m._e22 * n._e24 + m._e23 * n._e34 + m._e24 * n._e44;

	r._e31 = m._e31 * n._e11 + m._e32 * n._e21 + m._e33 * n._e31 + m._e34 * n._e41;
	r._e32 = m._e31 * n._e12 + m._e32 * n._e22 + m._e33 * n._e32 + m._e34 * n._e42;
	r._e33 = m._e31 * n._e13 + m._e32 * n._e23 + m._e33 * n._e33 + m._e34 * n._e43;
	r._e34 = m._e31 * n._e14 + m._e32 * n._e24 + m._e33 * n._e34 + m._e34 * n._e44;

	r._e41 = m._e41 * n._e11 + m._e42 * n._e21 + m._e43 * n._e31 + m._e44 * n._e41;
	r._e42 = m._e41 * n._e12 + m._e42 * n._e22 + m._e43 * n._e32 + m._e44 * n._e42;
	r._e43 = m._e41 * n._e13 + m._e42 * n._e23 + m._e43 * n._e33 + m._e44 * n._e43;
	r._e44 = m._e41 * n._e14 + m._e42 * n._e24 + m._e43 * n._e34 + m._e44 * n._e44;

	return r;
}

float Matrix_Determinant(float e_11, float e_12, float e_13,
						 float e_21, float e_22, float e_23,
						 float e_31, float e_32, float e_33) { 
	return e_11 * e_22 * e_33 + e_12 * e_23 * e_31 + e_13 * e_21 * e_32 - e_11 * e_23 * e_32 - e_12 * e_21 * e_33 - e_13 * e_22 * e_31;
}

float Matrix_Determinant(Matrix4x4 m) {
	return m._e11 * Matrix_Determinant(m._e22, m._e23, m._e24, m._e32, m._e33, m._e34, m._e42, m._e43, m._e44) -
		   m._e12 * Matrix_Determinant(m._e21, m._e23, m._e24, m._e31, m._e33, m._e34, m._e41, m._e43, m._e44) +
		   m._e13 * Matrix_Determinant(m._e21, m._e22, m._e24, m._e31, m._e32, m._e34, m._e41, m._e42, m._e44) -
		   m._e14 * Matrix_Determinant(m._e21, m._e22, m._e23, m._e31, m._e32, m._e33, m._e41, m._e42, m._e43);
}

Matrix4x4 Matrix_Inverse(Matrix4x4 m) {
	float det = Matrix_Determinant(m);
	if ( IsZero(det) )
		return m;

	Matrix4x4 r;
	float inv_det = 1.0f / det;

	r._e11 = Matrix_Determinant(m._e22, m._e23, m._e24, m._e32, m._e33, m._e34, m._e42, m._e43, m._e44) * inv_det;
	r._e12 = -Matrix_Determinant(m._e12, m._e13, m._e14, m._e32, m._e33, m._e34, m._e42, m._e43, m._e44) * inv_det;
	r._e13 = Matrix_Determinant(m._e12, m._e13, m._e14, m._e22, m._e23, m._e24, m._e42, m._e43, m._e44) * inv_det;
	r._e14 = -Matrix_Determinant(m._e12, m._e13, m._e14, m._e22, m._e23, m._e24, m._e32, m._e33, m._e34) * inv_det;
	r._e21 = -Matrix_Determinant(m._e21, m._e23, m._e24, m._e31, m._e33, m._e34, m._e41, m._e43, m._e44) * inv_det;
	r._e22 = Matrix_Determinant(m._e11, m._e13, m._e14, m._e31, m._e33, m._e34, m._e41, m._e43, m._e44) * inv_det;
	r._e23 = -Matrix_Determinant(m._e11, m._e13, m._e14, m._e21, m._e23, m._e24, m._e41, m._e43, m._e44) * inv_det;
	r._e24 = Matrix_Determinant(m._e11, m._e13, m._e14, m._e21, m._e23, m._e24, m._e31, m._e33, m._e34) * inv_det;
	r._e31 = Matrix_Determinant(m._e21, m._e22, m._e24, m._e31, m._e32, m._e34, m._e41, m._e42, m._e44) * inv_det;
	r._e32 = -Matrix_Determinant(m._e11, m._e12, m._e14, m._e31, m._e32, m._e34, m._e41, m._e42, m._e44) * inv_det;
	r._e33 = Matrix_Determinant(m._e11, m._e12, m._e14, m._e21, m._e22, m._e24, m._e41, m._e42, m._e44) * inv_det;
	r._e34 = -Matrix_Determinant(m._e11, m._e12, m._e14, m._e21, m._e22, m._e24, m._e31, m._e32, m._e34) * inv_det;
	r._e41 = -Matrix_Determinant(m._e21, m._e22, m._e23, m._e31, m._e32, m._e33, m._e41, m._e42, m._e43) * inv_det;
	r._e42 = Matrix_Determinant(m._e11, m._e12, m._e13, m._e31, m._e32, m._e33, m._e41, m._e42, m._e43) * inv_det;
	r._e43 = -Matrix_Determinant(m._e11, m._e12, m._e13, m._e21, m._e22, m._e23, m._e41, m._e42, m._e43) * inv_det;
	r._e44 = Matrix_Determinant(m._e11, m._e12, m._e13, m._e21, m._e22, m._e23, m._e31, m._e32, m._e33) * inv_det;

	return r;
}

Matrix4x4 MatrixTranslation(float _x, float _y, float _z) {
	Matrix4x4 m = { 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					_x, _y, _z, 1 };
	return m;
}

Matrix4x4 MatrixRotation_X(float _degree) {
	float rad = DegreesToradians(_degree);
	Matrix4x4 m = { 1, 0,			0,			0,
					0, cosf(rad),	-sinf(rad),	0,
					0, sinf(rad),	cosf(rad),	0,
					0, 0,			0,			1 };
	return m;
}

Matrix4x4 MatrixRotation_Y(float _degree) {
	float rad = DegreesToradians(_degree);
	Matrix4x4 m = { cosf(rad),	0,	sinf(rad),	0,
					0,			1,	0,			0,
					-sinf(rad),	0,	cosf(rad),	0,
					0,			0,	0,			1 };
	return m;
}

Matrix4x4 MatrixRotation_Z(float _degree) {
	float rad = DegreesToradians(_degree);
	Matrix4x4 m = { cosf(rad),	-sinf(rad),	0,	0,
					sinf(rad),	cosf(rad),	0,	0,
					0,			0,			1,	0,
					0,			0,			0,	1 };
	return m;
}

Matrix4x4 CreateProjectMatrix(float _nearPlane, float _farPlane, float _fovDegree, float _aspectRatio) {
	float fov_rad = DegreesToradians(_fovDegree);
	float yScale = 1.0f / tanf(0.5f*fov_rad);
	float xScale = yScale * _aspectRatio;
	float zFar, zNear;
	assert(_nearPlane!=0&& _farPlane > _nearPlane);
	zNear = _nearPlane;
	zFar = _farPlane;
	Matrix4x4 m = { xScale, 0,		 0,								0,
					0	  , yScale,	 0,								0,
					0	  , 0,		 zFar / (zFar - zNear),			1,
					0	  , 0,		 -(zFar*zNear) / (zFar-zNear),	0 };
	return m;
}

void DrawLineUsingShader(const Vertex4 &_start, const Vertex4 &_end, unsigned int *_buffer, float *_zBuffer = nullptr) {
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
	DrawParametricLine(screen_start.x, screen_start.y, screen_end.x, screen_end.y, _buffer,  _start.color, _end.color, _zBuffer, 0.0f);

}

Pixel2D CartesianToScreen(Vertex4 _v4) {
	Pixel2D screenPos;

	screenPos.x = (int)((_v4.x + 1.0f) *0.5f * RASTER_WIDTH);
	screenPos.y = (int)((1.0f - _v4.y) *0.5f * RASTER_HEIGHT);
	screenPos.depth = _v4.z;
	return screenPos;
}

Vertex4 ScreenToCartesian(Pixel2D _p2) {
	Vertex4 realPos;
	realPos.x = ((float)_p2.x / RASTER_WIDTH *2.0f - 1.0f);
	realPos.y = -((float)_p2.y / RASTER_HEIGHT *2.0f - 1.0f);
	realPos.z = _p2.depth;
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

void BetterBruteTriangle(const Tri _triangle, unsigned int *_buffer, unsigned int _color, float* _zBuffer) {

	Vertex4 copy_vert[3] = { _triangle.a, _triangle.b, _triangle.c };

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
	copy_vert[0].u /= copy_vert[0].w;
	copy_vert[1].u /= copy_vert[1].w;
	copy_vert[2].u /= copy_vert[2].w;
	copy_vert[0].v /= copy_vert[0].w;
	copy_vert[1].v /= copy_vert[1].w;
	copy_vert[2].v /= copy_vert[2].w;
	for (int curY = startY; curY < endY; curY++) {
		for (int curX = startX; curX < endX; curX++) {
			Pixel2D curScreenPos;
			curScreenPos.x = curX;
			curScreenPos.y = curY;

			Vertex4 curPoint = ScreenToCartesian(curScreenPos);
			Vertex4 bya = FindBarycentricPoint(curPoint, copy_vert);

			float Z = bya.z*copy_vert[0].z + bya.x*copy_vert[1].z + bya.y*copy_vert[2].z;


			float U = bya.z*copy_vert[0].u + bya.x*copy_vert[1].u + bya.y*copy_vert[2].u;
			float V = bya.z*copy_vert[0].v + bya.x*copy_vert[1].v + bya.y*copy_vert[2].v;
			float W = 1.0f / copy_vert[0].w * bya.z + 1.0f / copy_vert[1].w * bya.x + 1.0f / copy_vert[2].w * bya.y;
			U /= W;
			V /= W;
			if ((bya.x >= 0 && bya.x <= 1) &&
				(bya.y >= 0 && bya.y <= 1) &&
				(bya.z >= 0 && bya.z <= 1)) {

				unsigned int blendColor = ColorLerpTriangle(_triangle.a.color, _triangle.b.color, _triangle.c.color, bya.x, bya.y, bya.z);
				if ( PixelShader ) {
					PS_UVShader(blendColor, U, V);
				}
				DrawPoint(curX, curY, _buffer, blendColor, _zBuffer, Z);
			}
		}
	}
}