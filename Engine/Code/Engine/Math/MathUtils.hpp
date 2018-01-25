#pragma once
#include "Engine/Math/IntVector2.hpp"
	//General utility 
	float CosDegrees(float degrees);
	float SinDegrees(float degrees);
	float TanDegrees(float degrees);
	float ConvertRadiansToDegrees(float radians);
	float ConvertDegreesToRadians(float degrees); 
	int GetRandomIntLessThan(int maxValueNotInclusive);
	int GetRandomIntInRange(int minValueInclusive, int maxValueInclusive);
	float GetRandomFloatZeroToOne();
	float GetRandomFloatInRange(float minimumInclusive, float maximumInclusive);
	float RangeMapFloat(float inValue, float inMin, float inMax, float outMin, float outMax); //47 degrees farhenit in range 32-212  convert to range in 0-100 degrees celsius
	float ClampWithin(float value, float min, float max);
	float CalcShortestAngularDistance(float startDegrees, float endDegrees);
	float FastFloor(float floorNumber);
	float SmoothStep(float stepNumber);
	float SmoothStep5(float stepNumber);

	float CalculateManhattenDistance(IntVector2 start, IntVector2 end);
