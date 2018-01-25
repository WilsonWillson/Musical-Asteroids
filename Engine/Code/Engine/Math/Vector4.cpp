#include "Engine/Math/Vector4.hpp"
#include <math.h>

Vector4::Vector4()
	: x(0.f)
	, y(0.f)
	, z(0.f)
	, w(0.f)
{

}

Vector4::Vector4(float initialX, float initialY, float initialZ, float initialW)
	: x(initialX)
	, y(initialY)
	, z(initialZ)
	, w(initialW)
{

}

Vector4::Vector4(const Vector4& copy)
	: x(copy.x), y(copy.y), z(copy.z), w(copy.w)
{

}

Vector4::Vector4(Vector3 xyzCords, float initialW)
	: x(xyzCords.x)
	, y(xyzCords.y)
	, z(xyzCords.z)
	, w(initialW)
{

}

void Vector4::GetXYZ(float& out_x, float& out_y, float& out_z)
{
	out_x = x;
	out_y = y;
	out_z = z;
}

void Vector4::GetXYZW(float& out_x, float& out_y, float& out_z, float& out_w) const
{
	out_x = x;
	out_y = y;
	out_z = z;
	out_w = w;
}

const float* Vector4::GetAsFloatArray() const
{
	return &x;
}

float* Vector4::GetAsFloatArray()
{
	return &x;
}

float Vector4::CalcLength() const
{
	return (float)sqrt((x*x) + (y*y) + (z*z) + (w*w));
}

float Vector4::CalcLengthSquared() const
{
	return (float)(sqrt((x*x) + (y*y) + (z*z) + (w*w)) * sqrt((x*x) + (y*y) + (z*z) + (w*w)));
}

void Vector4::SetXYZ(float newX, float newY, float newZ, float newW)
{
	x = newX;
	y = newY;
	z = newZ;
	w = newW;
}

void Vector4::Normalize3D()
{
	float length = (float)sqrt((x*x) + (y *y));
	if (length != 0)
	{
		float invLength = (1.0f / length);
		x *= invLength;
		y *= invLength;
		z *= invLength;
	}
}

void Vector4::Normalize4D()
{
	float length = (float)sqrt((x*x) + (y *y));
	if (length != 0)
	{
		float invLength = (1.0f / length);
		x *= invLength;
		y *= invLength;
		z *= invLength;
		w *= invLength;
	}
}

void Vector4::SetLength(float newLength)
{
	Normalize4D();
	ScaleUniform(newLength);
}

void Vector4::ScaleUniform(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
}

void Vector4::ScaleNonUniform(const Vector4& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
	w *= perAxisScaleFactors.w;
}

void Vector4::InverseScaleNonUniform(const Vector4& perAxisDivisors)
{
	x *= perAxisDivisors.x;
	y *= perAxisDivisors.y;
	z *= perAxisDivisors.z;
	w *= perAxisDivisors.w;
}


Vector3 Vector4::xyz() const
{
	return Vector3(x, y, z);
}

void Vector4::operator-=(const Vector4& vectorToSubtract)
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
	w -= vectorToSubtract.w;
}

void Vector4::operator+=(const Vector4& vectorToAdd)
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
	w += vectorToAdd.w;
}

void Vector4::operator*=(const Vector4& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
	w *= perAxisScaleFactors.w;
}

void Vector4::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
}

const Vector4 Vector4::operator/(float inverseScale) const
{
	Vector4 newVector;
	newVector.x = inverseScale / x;
	newVector.y = inverseScale / y;
	newVector.z = inverseScale / z;
	newVector.w = inverseScale / w;
	return newVector;
}

const Vector4 Vector4::operator*(const Vector4& perAxisScaleFactors) const
{
	Vector4 newVector;
	newVector.x = perAxisScaleFactors.x * x;
	newVector.y = perAxisScaleFactors.y * y;
	newVector.z = perAxisScaleFactors.z * z;
	newVector.w = perAxisScaleFactors.w * w;
	return newVector;
}

const Vector4 Vector4::operator*(float scale) const
{
	Vector4 newVector;
	newVector.x = scale * x;
	newVector.y = scale * y;
	newVector.z = scale * z;
	newVector.w = scale * w;
	return newVector;
}

const Vector4 Vector4::operator-(const Vector4& vectorToSubtract) const
{
	Vector4 newVector;
	newVector.x = vectorToSubtract.x - x;
	newVector.y = vectorToSubtract.y - y;
	newVector.z = vectorToSubtract.z - z;
	newVector.w = vectorToSubtract.w - w;
	return newVector;
}

const Vector4 Vector4::operator+(const Vector4& vectorToAdd) const
{
	Vector4 newVector;
	newVector.x = vectorToAdd.x + x;
	newVector.y = vectorToAdd.y + y;
	newVector.z = vectorToAdd.z + z;
	newVector.w = vectorToAdd.w + w;
	return newVector;

}

bool Vector4::operator!=(const Vector4& vectorToNotEqual) const
{
	if (vectorToNotEqual.x != x || vectorToNotEqual.y != y || vectorToNotEqual.z != z || vectorToNotEqual.w != w)
	{
		return true;
	}
	return false;
}

bool Vector4::operator==(const Vector4& vectorToEqual) const
{
	if (vectorToEqual.x == x && vectorToEqual.y == y && vectorToEqual.z == z && vectorToEqual.w == w)
	{
		return true;
	}
	return false;
}
