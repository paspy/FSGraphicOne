#pragma once
#include <cmath>

// marcos
#define RASTER_WIDTH 500
#define RASTER_HEIGHT 500
#define NUM_PIXELS (RASTER_WIDTH*RASTER_HEIGHT)

#define VKNUM_1 0x31
#define VKNUM_2 0x32
#define VKNUM_3 0x33
#define VKNUM_4 0x34

// matrix3x3 define
typedef float Maxtrix3x3[3][3];

// vector classes
class Vector2D {
public:
	float x, y;
	Vector2D() {}
	Vector2D(float _r, float _s) {
		x = _r;
		y = _s;
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

