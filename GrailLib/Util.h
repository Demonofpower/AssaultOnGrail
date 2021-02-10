#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define PI 3.1415927f

#include <string>

struct Vector3
{
	float x, y, z;
};

//class Vector3
//{
//public:
//
//	float x, y, z;
//
//	Vector3()
//	{
//	};
//
//	Vector3(const float x, const float y, const float z) : x(x), y(y), z(z)
//	{
//	}
//
//	Vector3 operator +(const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
//	Vector3 operator -(const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
//	Vector3 operator *(const float& rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
//	Vector3 operator /(const float& rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }
//	Vector3& operator +=(const Vector3& rhs) { return *this = *this + rhs; }
//	Vector3& operator -=(const Vector3& rhs) { return *this = *this - rhs; }
//	Vector3& operator *=(const float& rhs) { return *this = *this * rhs; }
//	Vector3& operator /=(const float& rhs) { return *this = *this / rhs; }
//	float Length() const { return sqrtf(x * x + y * y + z * z); }
//	Vector3 Normalize() const { return *this * (1 / Length()); }
//	float Distance(const Vector3& rhs) const { return (*this - rhs).Length(); }
//};

struct Vector4
{
	float x, y, z, w;
};

namespace Util
{
	std::string boolToString(bool b);

	char* stringToCharPtr(std::string str);

	Vector3 Subtract(Vector3 src, Vector3 dst);
	float Magnitude(Vector3 vec);
	float Distance(Vector3 src, Vector3 dst);
	Vector3 CalcAngle(Vector3 src, Vector3 dst);
	bool WorldToScreen(Vector3 pos, Vector3& screen, float matrix[16], int windowWidth, int windowHeight);
}
