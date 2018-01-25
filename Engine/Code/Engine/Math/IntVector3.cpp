#include "Engine/Math/IntVector3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <math.h>

const double PI = 3.1415926535897932384626433832795;

const IntVector3 IntVector3::ZERO(0, 0, 0);

IntVector3::IntVector3()
	: x(0)
	, y(0)
	, z(0)
{

}

IntVector3::IntVector3(int initialX, int initialY, int initialZ)
	: x(initialX)
	, y(initialY)
	, z(initialZ)
{

}
IntVector3::IntVector3(const IntVector3& copy)
	: x(copy.x), y(copy.y), z(copy.z)
{

}
IntVector3::~IntVector3()
{

}

//IntVector2 Accessors
void IntVector3::GetXY(int& out_x, int& out_y, int& out_z) const
{
	out_x = x;
	out_y = y;
	out_z = z;
}


//-----------------------------------------------------------------------------------------------
const int* IntVector3::GetAsIntArray() const
{
	return &x;
}


//----------------------------------------------------------------------------------------------
int* IntVector3::GetAsIntArray()
{
	return &x;
}


//-----------------------------------------------------------------------------------------------
int IntVector3::CalcLength() const
{
	return (int)sqrt((x*x) + (y*y) + (z*z));
}


//-----------------------------------------------------------------------------------------------
int IntVector3::CalcLengthSquared() const
{
	return (int)(sqrt((x*x) + (y*y) + (z*z)) * sqrt((x*x) + (y*y) + (z*z)));
}


//IntVector2 Mutators
void IntVector3::SetXY(int newX, int newY, int newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}


//-----------------------------------------------------------------------------------------------
void IntVector3::Normalize()
{
	int length = (int)sqrt((x*x) + (y *y) + (z*z));
	if (length != 0)
	{
		int invLength = (1 / length);
		x *= invLength;
		y *= invLength;
		z *= invLength;
	}
}


//-----------------------------------------------------------------------------------------------
void IntVector3::SetLength(int newLength)
{
	Normalize();
	ScaleUniform(newLength);

}

//-----------------------------------------------------------------------------------------------
void IntVector3::ScaleUniform(int scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}


//-----------------------------------------------------------------------------------------------
void IntVector3::ScaleNonUniform(const IntVector3& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}


//-----------------------------------------------------------------------------------------------
void IntVector3::InverseScaleNonUniform(const IntVector3& perAxisDivisors)
{
	if (perAxisDivisors.x != 0 && perAxisDivisors.y != 0)
	{
		x *= (1 / perAxisDivisors.x);
		y *= (1 / perAxisDivisors.y);
		z *= (1 / perAxisDivisors.z);
	}
}


//Unit length 1 at a passed in angle (so 45 degrees means x = .707 y = .707)
//-----------------------------------------------------------------------------------------------
void IntVector3::SetUnitLengthAndYawDegrees(int yawDegrees)
{
	x = (int)cos(yawDegrees);
	y = (int)sin(yawDegrees);
	//z = (int)
}


//-----------------------------------------------------------------------------------------------
void IntVector3::SetUnitLengthAndYawRadians(int yawRadians)
{
	x = (int)cos(yawRadians);
	y = (int)sin(yawRadians);
}


//-----------------------------------------------------------------------------------------------
void IntVector3::SetLengthAndYawDegrees(int newLength, int yawDegrees)
{
	x = (int)(newLength * cos(yawDegrees));
	y = (int)(newLength * sin(yawDegrees));
}


//-----------------------------------------------------------------------------------------------
void IntVector3::SetLengthAndYawRadians(int newLength, int yawRadians)
{
	x = (int)(newLength * cos(yawRadians));
	y = (int)(newLength * sin(yawRadians));
}


//IntVector2 Operators
bool IntVector3::operator == (const IntVector3& vectorToEqual) const
{
	if (vectorToEqual.x == x && vectorToEqual.y == y && vectorToEqual.z == z)
	{
		return true;
	}
	return false;
}


//-----------------------------------------------------------------------------------------------
bool IntVector3::operator != (const IntVector3& vectorToNotEqual) const
{
	if (vectorToNotEqual.x != x || vectorToNotEqual.y != y || vectorToNotEqual.z != z)
	{
		return true;
	}
	return false;
}


//-----------------------------------------------------------------------------------------------
const IntVector3 IntVector3::operator + (const IntVector3& vectorToAdd) const
{
	IntVector3 newVector;
	newVector.x = vectorToAdd.x + x;
	newVector.y = vectorToAdd.y + y;
	newVector.z = vectorToAdd.z + z;
	return newVector;
}


//-----------------------------------------------------------------------------------------------
const IntVector3 IntVector3::operator - (const IntVector3& vectorToSubtract) const
{
	IntVector3 newVector;
	newVector.x = vectorToSubtract.x - x;
	newVector.y = vectorToSubtract.y - y;
	newVector.z = vectorToSubtract.z - z;
	return newVector;
}


//-----------------------------------------------------------------------------------------------
const IntVector3 IntVector3::operator * (int scale) const
{
	IntVector3 newVector;
	newVector.x = scale * x;
	newVector.y = scale * y;
	newVector.z = scale * z;
	return newVector;
}


//-----------------------------------------------------------------------------------------------
const IntVector3 IntVector3::operator * (const IntVector3& perAxisScaleFactors) const
{
	IntVector3 newVector;
	newVector.x = perAxisScaleFactors.x * x;
	newVector.y = perAxisScaleFactors.y * y;
	newVector.z = perAxisScaleFactors.z * z;
	return newVector;
}


//-----------------------------------------------------------------------------------------------
const IntVector3 IntVector3::operator / (int inverseScale) const
{
	IntVector3 newVector;
	newVector.x = inverseScale / x;
	newVector.y = inverseScale / y;
	newVector.z = inverseScale / z;
	return newVector;
}


//-----------------------------------------------------------------------------------------------
void IntVector3::operator *= (int scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}


//-----------------------------------------------------------------------------------------------
void IntVector3::operator *= (const IntVector3& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}


//-----------------------------------------------------------------------------------------------
void IntVector3::operator += (const IntVector3& vectorToAdd)
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
}


//-----------------------------------------------------------------------------------------------
void IntVector3::operator -= (const IntVector3& vectorToSubtract)
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
}


//IntVector2 Standalone friend functions
int CalcDistance(const IntVector3& positionA, const IntVector3& positionB)
{
	IntVector3 distance = positionB - positionA;
	return (int)sqrt((distance.x*distance.x) + (distance.y*distance.y) + (distance.z * distance.z));
}


//-----------------------------------------------------------------------------------------------
int CalcDistanceSquared(const IntVector3& posA, const IntVector3& posB)
{
	IntVector3 distance = posB - posA;
	int distanceNumber = (int)sqrt((distance.x*distance.x) + (distance.y*distance.y) + (distance.z *distance.z));
	return distanceNumber * distanceNumber;
}


//-----------------------------------------------------------------------------------------------
const IntVector3 operator * (int scale, const IntVector3& vectorToScale)
{
	IntVector3 newVector;
	newVector.x = scale * vectorToScale.x;
	newVector.y = (scale * vectorToScale.y);
	newVector.z = (scale * vectorToScale.z);
	return newVector;
}


//-----------------------------------------------------------------------------------------------
int DotProduct(const IntVector3& a, const IntVector3& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

