#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <math.h>

const double PI = 3.1415926535897932384626433832795;

const IntVector2 IntVector2::ZERO(0, 0);

	IntVector2::IntVector2()
		: x(0)
		, y(0)
	{

	}

	IntVector2::IntVector2(int initialX, int initialY)
		: x(initialX)
		, y(initialY)
	{

	}
	IntVector2::IntVector2(const IntVector2& copy)
		: x(copy.x), y(copy.y)
	{

	}
	IntVector2::~IntVector2()
	{

	}

//IntVector2 Accessors
void IntVector2::GetXY(int& out_x, int& out_y) const
{
	out_x = x;
	out_y = y;
}


//-----------------------------------------------------------------------------------------------
const int* IntVector2::GetAsIntArray() const
{
	return &x;
}


//----------------------------------------------------------------------------------------------
int* IntVector2::GetAsIntArray()
{
	return &x;
}


//-----------------------------------------------------------------------------------------------
int IntVector2::CalcLength() const
{
	return (int)sqrt((x*x) + (y*y));
}


//-----------------------------------------------------------------------------------------------
int IntVector2::CalcLengthSquared() const
{
	return (int)(sqrt((x*x) + (y*y)) * sqrt((x*x) + (y*y)));
}


/*
//-----------------------------------------------------------------------------------------------
int IntVector2::CalcAngleDegrees() const
{
	int radians = CalcAngleRadians();
	return (int)(ConvertRadiansToDegrees((int)radians));
}
*/

//-----------------------------------------------------------------------------------------------
int IntVector2::CalcAngleRadians() const
{
	return (int)atan2(y, x);
}


//IntVector2 Mutators
void IntVector2::SetXY(int newX, int newY)
{
	x = newX;
	y = newY;
}


//-----------------------------------------------------------------------------------------------
void IntVector2::Rotate90Degrees()
{
	int oldX = x;
	int oldY = y;
	x = -oldY;
	y = oldX;
}


//-----------------------------------------------------------------------------------------------
void IntVector2::RotateNegative90Degrees()
{
	int oldX = x;
	int oldY = y;
	x = oldY;
	y = -oldX;
}

//-----------------------------------------------------------------------------------------------
void IntVector2::RotateDegrees(int degrees)
{
	int radians = (int)sqrt((x * x) + (y * y));
	int mydegrees = (int)(atan2(y, x) * 180 / PI);
	mydegrees += degrees;
	x = radians * (int)cos(mydegrees);
	y = radians * (int)sin(mydegrees);
}

/*
//-----------------------------------------------------------------------------------------------
void IntVector2::RotateRadians(int radians)
{
	int degrees = (int)ConvertRadiansToDegrees((int)radians);
	RotateDegrees(degrees);
}
*/

//-----------------------------------------------------------------------------------------------
void IntVector2::Normalize()
{
	int length = (int)sqrt((x*x) + (y *y));
	if (length != 0)
	{
		int invLength = (1 / length);
		x *= invLength;
		y *= invLength;
	}
}


//-----------------------------------------------------------------------------------------------
void IntVector2::SetLength(int newLength)
{
	Normalize();
	ScaleUniform(newLength);

}

//-----------------------------------------------------------------------------------------------
void IntVector2::ScaleUniform(int scale)
{
	x *= scale;
	y *= scale;
}


//-----------------------------------------------------------------------------------------------
void IntVector2::ScaleNonUniform(const IntVector2& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}


//-----------------------------------------------------------------------------------------------
void IntVector2::InverseScaleNonUniform(const IntVector2& perAxisDivisors)
{
	if (perAxisDivisors.x != 0 && perAxisDivisors.y != 0)
	{
		x *= (1 / perAxisDivisors.x);
		y *= (1 / perAxisDivisors.y);
	}
}


//Unit length 1 at a passed in angle (so 45 degrees means x = .707 y = .707)
//-----------------------------------------------------------------------------------------------
void IntVector2::SetUnitLengthAndYawDegrees(int yawDegrees)
{
	x = (int)cos(yawDegrees);
	y = (int)sin(yawDegrees);
}


//-----------------------------------------------------------------------------------------------
void IntVector2::SetUnitLengthAndYawRadians(int yawRadians)
{
	x = (int)cos(yawRadians);
	y = (int)sin(yawRadians);
}


//-----------------------------------------------------------------------------------------------
void IntVector2::SetLengthAndYawDegrees(int newLength, int yawDegrees)
{
	x = (int)(newLength * cos(yawDegrees));
	y = (int)(newLength * sin(yawDegrees));
}


//-----------------------------------------------------------------------------------------------
void IntVector2::SetLengthAndYawRadians(int newLength, int yawRadians)
{
	x = (int)(newLength * cos(yawRadians));
	y = (int)(newLength * sin(yawRadians));
}


//IntVector2 Operators
bool IntVector2::operator == (const IntVector2& vectorToEqual) const
{
	if (vectorToEqual.x == x && vectorToEqual.y == y)
	{
		return true;
	}
	return false;
}


//-----------------------------------------------------------------------------------------------
bool IntVector2::operator != (const IntVector2& vectorToNotEqual) const
{
	if (vectorToNotEqual.x != x || vectorToNotEqual.y != y)
	{
		return true;
	}
	return false;
}


bool IntVector2::operator<(const IntVector2& rhs) const
{
	if (y < rhs.y)
		return true;
	if (y > rhs.y)
		return false;
	return x < rhs.x;
}

//-----------------------------------------------------------------------------------------------
const IntVector2 IntVector2::operator + (const IntVector2& vectorToAdd) const
{
	IntVector2 newVector;
	newVector.x = vectorToAdd.x + x;
	newVector.y = vectorToAdd.y + y;
	return newVector;
}


//-----------------------------------------------------------------------------------------------
const IntVector2 IntVector2::operator - (const IntVector2& vectorToSubtract) const
{
	IntVector2 newVector;
	newVector.x = vectorToSubtract.x - x;
	newVector.y = vectorToSubtract.y - y;
	return newVector;
}


//-----------------------------------------------------------------------------------------------
const IntVector2 IntVector2::operator * (int scale) const
{
	IntVector2 newVector;
	newVector.x = scale * x;
	newVector.y = scale * y;
	return newVector;
}


//-----------------------------------------------------------------------------------------------
const IntVector2 IntVector2::operator * (const IntVector2& perAxisScaleFactors) const
{
	IntVector2 newVector;
	newVector.x = perAxisScaleFactors.x * x;
	newVector.y = perAxisScaleFactors.y * y;
	return newVector;
}


//-----------------------------------------------------------------------------------------------
const IntVector2 IntVector2::operator / (int inverseScale) const
{
	IntVector2 newVector;
	newVector.x = inverseScale / x;
	newVector.y = inverseScale / y;
	return newVector;
}


//-----------------------------------------------------------------------------------------------
void IntVector2::operator *= (int scale)
{
	x *= scale;
	y *= scale;
}


//-----------------------------------------------------------------------------------------------
void IntVector2::operator *= (const IntVector2& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}


//-----------------------------------------------------------------------------------------------
void IntVector2::operator += (const IntVector2& vectorToAdd)
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
}


//-----------------------------------------------------------------------------------------------
void IntVector2::operator -= (const IntVector2& vectorToSubtract)
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
}


//IntVector2 Standalone friend functions
int CalcDistance(const IntVector2& positionA, const IntVector2& positionB)
{
	IntVector2 distance = positionB - positionA;
	return (int)sqrt((distance.x*distance.x) + (distance.y*distance.y));
}


//-----------------------------------------------------------------------------------------------
int CalcDistanceSquared(const IntVector2& posA, const IntVector2& posB)
{
	IntVector2 distance = posB - posA;
	int distanceNumber = (int)sqrt((distance.x*distance.x) + (distance.y*distance.y));
	return distanceNumber * distanceNumber;
}


//-----------------------------------------------------------------------------------------------
const IntVector2 operator * (int scale, const IntVector2& vectorToScale)
{
	IntVector2 newVector;
	newVector.x = scale * vectorToScale.x;
	newVector.y = (scale * vectorToScale.y);
	return newVector;
}


//-----------------------------------------------------------------------------------------------
int DotProduct(const IntVector2& a, const IntVector2& b)
{
	return (a.x * b.x + a.y * b.y);
}


