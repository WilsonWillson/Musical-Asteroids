#include "Engine/Math/Vector3.hpp"
#include <math.h>

Vector3::Vector3()
	: x(0.f)
	, y(0.f)
	, z(0.f)
{

}

Vector3::Vector3(float initialX, float initialY, float initialZ)
	: x(initialX)
	, y(initialY)
	, z(initialZ)
{
	
}

Vector3::Vector3(const Vector3& copy)
	: x(copy.x), y(copy.y), z(copy.z)
{

}

void Vector3::GetXYZ(float& out_x, float& out_y, float& out_z) const
{
	out_x = x;
	out_y = y;
	out_z = z;
}

const float* Vector3::GetAsFloatArray() const
{
	return &x;
}

float* Vector3::GetAsFloatArray()
{
	return &x;
}

float Vector3::CalcLength() const
{
	return (float)sqrt((x*x) + (y*y) + (z*z));
}

float Vector3::CalcLengthSquared() const
{
	return (float)(sqrt((x*x) + (y*y) + (z*z)) * sqrt((x*x) + (y*y) + (z*z)));
}

void Vector3::SetXYZ(float newX, float newY, float newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}

void Vector3::Normalize()
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

void Vector3::SetLength(float newLength)
{
	Normalize();
	ScaleUniform(newLength);
}

void Vector3::ScaleUniform(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}

void Vector3::ScaleNonUniform(const Vector3& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}

void Vector3::InverseScaleNonUniform(const Vector3& perAxisDivisors)
{
	if (perAxisDivisors.x != 0 && perAxisDivisors.y != 0)
	{
		x *= (1 / perAxisDivisors.x);
		y *= (1 / perAxisDivisors.y);
		z *= (1 / perAxisDivisors.z);
	}
}

void Vector3::operator-=(const Vector3& vectorToSubtract)
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
}

void Vector3::operator+=(const Vector3& vectorToAdd)
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
}

void Vector3::operator*=(const Vector3& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;

}

void Vector3::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}

const Vector3 Vector3::operator/(float inverseScale) const
{
	Vector3 newVector;
	newVector.x = x/inverseScale;
	newVector.y = y/inverseScale; 
	newVector.z = z/inverseScale;
	return newVector;
}

const Vector3 Vector3::operator*(const Vector3& perAxisScaleFactors) const
{
	Vector3 newVector;
	newVector.x = perAxisScaleFactors.x * x;
	newVector.y = perAxisScaleFactors.y * y;
	newVector.z = perAxisScaleFactors.z * z;
	return newVector;
}

const Vector3 Vector3::operator*(float scale) const
{
	Vector3 newVector;
	newVector.x = scale * x;
	newVector.y = scale * y;
	newVector.z = scale * z;
	return newVector;
}

const Vector3 Vector3::operator-(const Vector3& vectorToSubtract) const
{
	Vector3 newVector;
	newVector.x = x - vectorToSubtract.x;
	newVector.y = y - vectorToSubtract.y;
	newVector.z = z - vectorToSubtract.z;
	return newVector;
}

const Vector3 Vector3::operator+(const Vector3& vectorToAdd) const
{
	Vector3 newVector;
	newVector.x = vectorToAdd.x + x;
	newVector.y = vectorToAdd.y + y;
	newVector.z = vectorToAdd.z + z;
	return newVector;
}

bool Vector3::operator!=(const Vector3& vectorToNotEqual) const
{
	if (vectorToNotEqual.x != x || vectorToNotEqual.y != y || vectorToNotEqual.z != z)
	{
		return true;
	}
	return false;
}

bool Vector3::operator==(const Vector3& vectorToEqual) const
{
	if (vectorToEqual.x == x && vectorToEqual.y == y && vectorToEqual.z == z)
	{
		return true;
	}
	return false;
}

//Vector3 Standalone friend functions
float CalcDistance(const Vector3& positionA, const Vector3& positionB)
{
	Vector3 distance = positionB - positionA;
	return (float)sqrt((distance.x*distance.x) + (distance.y*distance.y) + (distance.z * distance.z));
}


//-----------------------------------------------------------------------------------------------
float CalcDistanceSquared(const Vector3& posA, const Vector3& posB)
{
	Vector3 distance = posB - posA;
	float distanceNumber = (float)sqrt((distance.x*distance.x) + (distance.y*distance.y) + (distance.z * distance.z));
	return distanceNumber * distanceNumber;
}

/*
float CalcDistanceBetweenPoints(float pointA, float pointB)
{
	return pointB - pointA;
}*/

//-----------------------------------------------------------------------------------------------
const Vector3 operator * (float scale, const Vector3& vectorToScale)
{
	Vector3 newVector;
	newVector.x = scale * vectorToScale.x;
	newVector.y = (scale * vectorToScale.y);
	newVector.z = (scale * vectorToScale.z);
	return newVector;
}


//-----------------------------------------------------------------------------------------------
float DotProduct(const Vector3& a, const Vector3& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

Vector3 Vector3::Cross(Vector3 const &other) const
{
	return Vector3( y * other.z - z * other.y, -x * other.z + z * other.x, x * other.y - y * other.x);
}