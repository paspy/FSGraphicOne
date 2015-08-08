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
	// x y z w
	float xyzw[4];
	unsigned int color;
}*Vertex4_ptr;

typedef struct Pixel2D {
	int x, y;
}*Pixel2D_ptr;

typedef unsigned int Pixel;

typedef union Matrix3x3 {
	float e[9];

	struct {
		float _e11, _e12, _e13;
		float _e21, _e22, _e23;
		float _e31, _e32, _e33;
	};
}*Matrix3x3_ptr;

typedef union Matrix4x4 {
	float e[16];

	struct {
		float _e11, _e12, _e13, _e14;
		float _e21, _e22, _e23, _e24;
		float _e31, _e32, _e33, _e34;
		float _e41, _e42, _e43, _e44;
	};
}*Matrix4x4_ptr;

// vector classes
class Vector2D {
public:
	float x, y;
	unsigned int color;
	Vector2D() {}
	Vector2D(float _r, float _s) {
		x = _r;
		y = _s;
		color = 0xFFFFFFFF;
	}
	Vector2D& Set(float _r, float _s) {
		x = _r;
		y = _s;
		return (*this);
	}

	float& operator [](long _idx) {
		return ((&x)[_idx]);
	}

	const float& operator [](long _idx) const {
		return ((&x)[_idx]);
	}

	Vector2D& operator +=(const Vector2D& _v) {
		x += _v.x;
		y += _v.y;
		return (*this);
	}

	Vector2D& operator -=(const Vector2D& _v) {
		x -= _v.x;
		y -= _v.y;
		return (*this);
	}

	Vector2D& operator *=(float _t) {
		x *= _t;
		y *= _t;
		return (*this);
	}

	Vector2D& operator /=(float _t) {
		float f = 1.0F / _t;
		x *= f;
		y *= f;
		return (*this);
	}

	Vector2D& operator &=(const Vector2D& _v) {
		x *= _v.x;
		y *= _v.y;
		return (*this);
	}

	Vector2D operator -() const {
		return (Vector2D(-x, -y));
	}

	Vector2D operator +(const Vector2D& _v) const {
		return (Vector2D(x + _v.x, y + _v.y));
	}

	Vector2D operator -(const Vector2D& _v) const {
		return (Vector2D(x - _v.x, y - _v.y));
	}

	Vector2D operator *(float _t) const {
		return (Vector2D(x * _t, y * _t));
	}

	Vector2D operator /(float _t) const {
		float f = 1.0F / _t;
		return (Vector2D(x * f, y * f));
	}

	float operator *(const Vector2D& _v) const {
		return (x * _v.x + y * _v.y);
	}

	Vector2D operator &(const Vector2D& _v) const {
		return (Vector2D(x * _v.x, y * _v.y));
	}

	bool operator ==(const Vector2D& _v) const {
		return ((x == _v.x) && (y == _v.y));
	}

	bool operator !=(const Vector2D& _v) const {
		return ((x != _v.x) || (y != _v.y));
	}

	Vector2D& Normalize() {
		return (*this /= sqrtf(x * x + y * y));
	}

	Vector2D& Rotate(float angle) {
		float s = sinf(angle);
		float c = cosf(angle);
		float nx = c * x - s * y;
		float ny = s * x + c * y;
		x = nx;
		y = ny;
		return (*this);
	}

	Vector2D& Tranlate(const Vector2D& _v) {
		x += (_v.x - x);
		y += (_v.y - y);
		return (*this);
	}
	
	Vector2D& Scale(const Vector2D& _v) {
		x *= _v.x;
		y *= _v.y;
		return (*this);
	}
};


class Point2D : public Vector2D {
public:
	Point2D() {}
	Point2D(float _r, float _s) : Vector2D(_r, _s) {}
	Point2D& operator =(const Vector2D& _v) {
		x = _v.x;
		y = _v.y;
		return (*this);
	}
	Point2D& operator *=(float _t) {
		x *= _t;
		y *= _t;
		return (*this);
	}
	Point2D& operator /=(float _t) {
		float f = 1.0F / _t;
		x *= f;
		y *= f;
		return (*this);
	}
	Point2D operator -() const {
		return (Point2D(-x, -y));
	}
	Point2D operator +(const Vector2D& _v) const {
		return (Point2D(x + _v.x, y + _v.y));
	}
	Point2D operator -(const Vector2D& _v) const {
		return (Point2D(x - _v.x, y - _v.y));
	}
	Vector2D operator -(const Point2D& _p) const {
		return (Vector2D(x - _p.x, y - _p.y));
	}
	Point2D operator *(float _t) const {
		return (Point2D(x * _t, y * _t));
	}
	Point2D operator /(float _t) const {
		float f = 1.0F / _t;
		return (Point2D(x * f, y * f));
	}
};


inline Vector2D operator *(float _t, const Vector2D& _v) {
	return (Vector2D(_t * _v.x, _t * _v.y));
}

inline Point2D operator *(float _t, const Point2D& _p) {
	return (Point2D(_t * _p.x, _t * _p.y));
}

inline float Dot(const Vector2D& v1, const Vector2D& v2) {
	return (v1 * v2);
}

inline float Magnitude(const Vector2D& _v) {
	return (sqrtf(_v.x * _v.x + _v.y * _v.y));
}

inline float InverseMag(const Vector2D& _v) {
	return (1.0F / sqrtf(_v.x * _v.x + _v.y * _v.y));
}

inline float SquaredMag(const Vector2D& _v) {
	return (_v.x * _v.x + _v.y * _v.y);
}

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
int Lerp_(unsigned int _A, unsigned int _B, float _ratio);
unsigned int ColorLerp(unsigned int _A, unsigned int _B, float _ratio);
void DrawPoint(const unsigned int _x, const unsigned int _y, unsigned int *_buffer, unsigned int _color);
void DrawBresehamLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color);
void DrawMidpointLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _color);
void DrawParametricLine(int _x0, int _y0, int _x1, int _y1, unsigned int *_buffer, unsigned int _StartColor, unsigned int _EndColor);
void DrawLineUsingShader(const Vertex4 _start, const Vertex4 _end, unsigned int *_buffer);
void MultiplyVertexByMatrix(Vertex4 &_vertex4, Matrix4x4 _worldMatrix);
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

int Lerp_(unsigned int _A, unsigned int _B, float _ratio) {
	return (int)((((float)_B - (float)_A) * _ratio) + (float)_A);
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

	unsigned int newA = (unsigned int)Lerp_(startA, endA, _ratio) << 24;
	unsigned int newR = ((unsigned int)Lerp_(startR, endR, _ratio)) << 16;
	unsigned int newG = ((unsigned int)Lerp_(startG, endG, _ratio)) << 8;
	unsigned int newB = (unsigned int)Lerp_(startB, endB, _ratio);

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

//void seedFill(Pixel2D _pixel, unsigned int _color) {
//
//	unsigned int currentColor;
//	if ( (currentColor.red() == old_color[0]) &&
//		(currentColor.green() == old_color[1]) &&
//		(currentColor.blue() = old_color[2]) ) {
//
//		DrawPoint(_v4.xyzw[x])
//
//		for ( i = 0; i<8; i++ ) {
//			seedFill(x + d8[i].x_off, y + d8[i].x_off, old_color, new_color);
//		}
//	}
//
//}

void MultiplyVertexByMatrix(Vertex4 &_v4, Matrix4x4 _m4) {
	float x, y, z, w;
	x = _v4.xyzw[0] * _m4._e11 + _v4.xyzw[1] * _m4._e21 + _v4.xyzw[2] * _m4._e31 + _v4.xyzw[3] * _m4._e41;
	y = _v4.xyzw[0] * _m4._e12 + _v4.xyzw[1] * _m4._e22 + _v4.xyzw[2] * _m4._e32 + _v4.xyzw[3] * _m4._e42;
	z = _v4.xyzw[0] * _m4._e13 + _v4.xyzw[1] * _m4._e23 + _v4.xyzw[2] * _m4._e33 + _v4.xyzw[3] * _m4._e43;
	w = _v4.xyzw[0] * _m4._e14 + _v4.xyzw[1] * _m4._e24 + _v4.xyzw[2] * _m4._e34 + _v4.xyzw[3] * _m4._e44;
	_v4.xyzw[0] = x;
	_v4.xyzw[1] = y;
	_v4.xyzw[2] = z;
	_v4.xyzw[3] = w;
}

Matrix4x4 MatrixRotation_Z(float _degree) {
	float rad = DegreesToradians(_degree);
	Matrix4x4 m = { cosf(rad),	-sinf(rad),	0,	0,
		sinf(rad),	cosf(rad),	0,	0,
		0,			0,			1,	0,
		0,			0,			0,	1 };
	return m;
}

void DrawLineUsingShader(const Vertex4 _start, const Vertex4 _end, unsigned int *_buffer) {
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

	//for ( numPixels ) {
	//	Pixel copyColor = currColor; // Just like a Vertex, copy original. 
	//	if ( PixelShader ) PixelShader(copyColor); // Modify copy. 
	//	PlotPixel(currX, currY, copyColor); // Display the copy.
	//}

}

Pixel2D CartesianToScreen(Vertex4 _v4) {
	Pixel2D screenPos;

	screenPos.x = (int)((_v4.xyzw[0] + 1.0f) *0.5f * RASTER_WIDTH);
	screenPos.y = (int)((1.0f - _v4.xyzw[1]) *0.5f * RASTER_HEIGHT);

	return screenPos;
}

