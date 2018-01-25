#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <math.h>


const float fPI = 3.1415926535897932384626433832795f;

const Vector2 Vector2::ZERO(0.f, 0.f);

//-------------------Based on code given by Squirrel Eiserloh-------------------------------------
Vector2::Vector2()
	: x( 0.f )
	, y( 0.f )
{
}

Vector2::Vector2(const Vector2& copy)
: x(copy.x), y(copy.y)
{
}

//-----------------------------------------------------------------------------------------------
Vector2::Vector2( float initialX, float initialY )
	: x( initialX )
	, y( initialY )
{
}


//Vector2 Accessors
void Vector2::GetXY(float& out_x, float& out_y) const
{
	out_x = x;
	out_y = y;
}


//-----------------------------------------------------------------------------------------------
const float* Vector2::GetAsFloatArray() const
{
	return &x;
}


//----------------------------------------------------------------------------------------------
float* Vector2::GetAsFloatArray()
{
	return &x;
}


//-----------------------------------------------------------------------------------------------
float Vector2::CalcLength() const
{
	return (float) sqrt((x*x) + (y*y));
}


//-----------------------------------------------------------------------------------------------
float Vector2::CalcLengthSquared() const
{
	return (float) (sqrt((x*x) + (y*y)) * sqrt((x*x) + (y*y)));
}



//-----------------------------------------------------------------------------------------------
float Vector2::CalcAngleDegrees() const
{
		float radians = CalcAngleRadians();
		return ConvertRadiansToDegrees(radians);
}


//-----------------------------------------------------------------------------------------------
float Vector2::CalcAngleRadians() const
{
     return (float)atan2(y, x); 
}


//Vector2 Mutators
void Vector2::SetXY(float newX, float newY)
{
	x = newX;
	y = newY;
}


//-----------------------------------------------------------------------------------------------
void Vector2::Rotate90Degrees()
{
	float oldX = x;
	float oldY = y;
	x = -oldY;
	y = oldX;
}


//-----------------------------------------------------------------------------------------------
void Vector2::RotateNegative90Degrees()
{
	float oldX = x;
	float oldY = y;
	x = oldY;
	y = -oldX;
}

//-----------------------------------------------------------------------------------------------
void Vector2::RotateDegrees(float degrees)
{
	float radians = (float)sqrt((x * x) + (y * y));
	float mydegrees = (float)atan2(y, x) * 180 / fPI;;
	mydegrees +=degrees;
	x = radians * CosDegrees(mydegrees);
	y = radians * SinDegrees(mydegrees);
}


//-----------------------------------------------------------------------------------------------
void Vector2::RotateRadians(float radians)
{
	float degrees = ConvertRadiansToDegrees(radians);
	RotateDegrees(degrees);
}


//-----------------------------------------------------------------------------------------------
void Vector2::Normalize()
{
	float length = (float)sqrt((x*x) + (y *y));
	if (length != 0)
	{
		float invLength = (1.0f / length);
		x *= invLength;
		y *= invLength;
	}
}


//-----------------------------------------------------------------------------------------------
void Vector2::SetLength(float newLength)
{
	Normalize();
	ScaleUniform(newLength);
	
}

//-----------------------------------------------------------------------------------------------
void Vector2::ScaleUniform(float scale)
{
		x *= scale;
		y *= scale;
}


//-----------------------------------------------------------------------------------------------
void Vector2::ScaleNonUniform(const Vector2& perAxisScaleFactors)
{
		x *= perAxisScaleFactors.x;
		y *= perAxisScaleFactors.y;
}


//-----------------------------------------------------------------------------------------------
void Vector2::InverseScaleNonUniform(const Vector2& perAxisDivisors)
{
	if (perAxisDivisors.x != 0 && perAxisDivisors.y != 0)
	{
		x *= (1 / perAxisDivisors.x);
		y *= (1 / perAxisDivisors.y);
	}
}


//Unit length 1 at a passed in angle (so 45 degrees means x = .707 y = .707)
//-----------------------------------------------------------------------------------------------
void Vector2::SetUnitLengthAndYawDegrees(float yawDegrees)
{
	x = (float) CosDegrees(yawDegrees);
	y = (float) SinDegrees(yawDegrees);
}


//-----------------------------------------------------------------------------------------------
void Vector2::SetUnitLengthAndYawRadians(float yawRadians)
{
	x = (float) cos(yawRadians);
	y = (float) sin(yawRadians);
}


//-----------------------------------------------------------------------------------------------
void Vector2::SetLengthAndYawDegrees(float newLength, float yawDegrees)
{
	x = (float)(newLength * CosDegrees(yawDegrees));
	y = (float)(newLength * SinDegrees(yawDegrees));
}


//-----------------------------------------------------------------------------------------------
void Vector2::SetLengthAndYawRadians(float newLength, float yawRadians)
{
	x = (float)(newLength * cos(yawRadians));
	y = (float)(newLength * sin(yawRadians));
}


//Vector2 Operators
bool Vector2::operator == (const Vector2& vectorToEqual) const
{
	if (vectorToEqual.x == x && vectorToEqual.y == y)
	{
		return true;
	}
	return false;
}


//-----------------------------------------------------------------------------------------------
bool Vector2::operator != (const Vector2& vectorToNotEqual) const
{
	if (vectorToNotEqual.x != x || vectorToNotEqual.y != y)
	{
		return true;
	}
	return false;
}


//-----------------------------------------------------------------------------------------------
const Vector2 Vector2::operator + (const Vector2& vectorToAdd) const
{
	 Vector2 newVector;
	 newVector.x = vectorToAdd.x + x;
	 newVector.y = vectorToAdd.y + y;
	 return newVector;
}


//-----------------------------------------------------------------------------------------------
const Vector2 Vector2::operator - (const Vector2& vectorToSubtract) const
{
	Vector2 newVector;
	newVector.x = x- vectorToSubtract.x;
	newVector.y = y- vectorToSubtract.y;
	return newVector;
}

const Vector2 Vector2::operator - (const float scale) const
{
	Vector2 newVector;
	newVector.x = scale - x;
	newVector.y = scale - y;
	return newVector;
}

//-----------------------------------------------------------------------------------------------
const Vector2 Vector2::operator * (float scale) const
{
	Vector2 newVector;
	newVector.x = scale * x;
	newVector.y = scale * y;
	return newVector;
}


//-----------------------------------------------------------------------------------------------
const Vector2 Vector2::operator * (const Vector2& perAxisScaleFactors) const
{
	Vector2 newVector;
	newVector.x = perAxisScaleFactors.x * x;
	newVector.y = perAxisScaleFactors.y * y;
	return newVector;
}


//-----------------------------------------------------------------------------------------------
const Vector2 Vector2::operator / (float inverseScale) const
{
	Vector2 newVector;
	newVector.x = inverseScale / x;
	newVector.y = inverseScale / y;
	return newVector;
}


//-----------------------------------------------------------------------------------------------
void Vector2::operator *= (float scale)
{
		x *= scale;
		y *= scale; 
}


//-----------------------------------------------------------------------------------------------
void Vector2::operator *= (const Vector2& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}


//-----------------------------------------------------------------------------------------------
void Vector2::operator += (const Vector2& vectorToAdd)
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
}


//-----------------------------------------------------------------------------------------------
void Vector2::operator -= (const Vector2& vectorToSubtract)
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
}


//Vector2 Standalone friend functions
float CalcDistance(const Vector2& positionA, const Vector2& positionB)
{
	Vector2 distance = positionB - positionA;
	return (float)sqrt((distance.x*distance.x) + (distance.y*distance.y));
}


//-----------------------------------------------------------------------------------------------
float CalcDistanceSquared(const Vector2& posA, const Vector2& posB)
{
	Vector2 distance = posB - posA;
	float distanceNumber = (float)sqrt((distance.x*distance.x) + (distance.y*distance.y));
	return distanceNumber * distanceNumber;
}

/*
float CalcDistanceBetweenPoints(float pointA, float pointB)
{
	return pointB - pointA;
}*/

//-----------------------------------------------------------------------------------------------
const Vector2 operator * (float scale, const Vector2& vectorToScale)
{
	Vector2 newVector;
	 newVector.x = scale * vectorToScale.x;
	 newVector.y = (scale * vectorToScale.y);
	 return newVector;
}


//-----------------------------------------------------------------------------------------------
float DotProduct(const Vector2& a, const Vector2& b)
{
	return (a.x * b.x + a.y * b.y);
}


