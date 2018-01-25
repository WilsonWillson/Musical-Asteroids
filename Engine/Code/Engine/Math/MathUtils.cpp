//Based on code given by Squirrel Eiserloh
#include <stdlib.h>
#include <math.h>
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/MathUtils.hpp"

const float fPI = 3.1415926535897932384626433832795f;

//-----------------------------------------------------------------------------------------------
float CosDegrees(float degrees)
{
	float radians = ConvertDegreesToRadians(degrees);
	return (float)cos(radians);
}

//-----------------------------------------------------------------------------------------------
float SinDegrees(float degrees)
{
	float radians = ConvertDegreesToRadians(degrees);
	return (float)sin(radians);
}

//-----------------------------------------------------------------------------------------------
float TanDegrees(float degrees)
{
	float radians = ConvertDegreesToRadians(degrees);
	return (float)(sin(radians)/cos(radians));
}

//-----------------------------------------------------------------------------------------------
float ConvertRadiansToDegrees(float radians) 
{
	return radians*(180.f / fPI);
}

//-----------------------------------------------------------------------------------------------
float ConvertDegreesToRadians(float degrees)
{
	return degrees * (fPI / 180.f);
}

//-----------------------------------------------------------------------------------------------
float GetRandomFloatZeroToOne()
{
	int randomNumber = rand();
	return (float)randomNumber / (float)RAND_MAX;
}

//-----------------------------------------------------------------------------------------------
int GetRandomIntLessThan(int maxValueNotInclusive)
{
	return rand() % maxValueNotInclusive;
}

//-----------------------------------------------------------------------------------------------
int GetRandomIntInRange(int minValueInclusive, int maxValueInclusive)
{
	int randInt = GetRandomIntLessThan(maxValueInclusive + 1);
	return randInt + minValueInclusive;
}

//-----------------------------------------------------------------------------------------------
float GetRandomFloatInRange(float minInclusive, float maxInclusive)
{
	float randomZeroToOne = GetRandomFloatZeroToOne();
	return minInclusive + ((maxInclusive - minInclusive) * randomZeroToOne);
}

float RangeMapFloat(float inValue, float inMin, float inMax, float outMin, float outMax)
{
	float outValue = (outMax + outMin)/2;
	float inRange = inMax - inMin;
	float outRange = outMax - outMin;
	if (inRange != 0.f) {
		float inValuePercent = (inValue - inMin) * (1.f / inRange);
		outValue = outMin + (outRange * inValuePercent);
	}
	return outValue;
}

float ClampWithin(float value, float min, float max)
{
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

float CalcShortestAngularDistance(float startDegrees, float endDegrees)
{
	float angularDist = endDegrees - startDegrees;
	while (angularDist > 180.f)
		angularDist -= 360.f;
	while (angularDist < -180.f)
		angularDist += 360;
	return angularDist;
}

float FastFloor(float floorNumber)
{
	return (float)floor(floorNumber);
}

float SmoothStep(float stepNumber)
{
	return (3 * stepNumber * stepNumber) - (2 * stepNumber * stepNumber * stepNumber);
}

float SmoothStep5(float stepNumber)
{
	return (6 * stepNumber * stepNumber * stepNumber * stepNumber * stepNumber) - (15 * stepNumber * stepNumber * stepNumber * stepNumber) + (10 * stepNumber  * stepNumber * stepNumber);
}

float CalculateManhattenDistance(IntVector2 start, IntVector2 end)
{
	return (float)(abs(end.x - start.x) + abs(end.y - start.y));
}

