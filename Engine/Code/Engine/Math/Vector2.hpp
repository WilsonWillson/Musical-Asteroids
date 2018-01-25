#pragma once


//-----------------------------------------------------------------------------------------------
class Vector2
{
public:
	float x;
	float y;
public:

	static const Vector2 ZERO;
	Vector2();
	Vector2( float initialX, float initialY );
	Vector2(const Vector2& copy);

	//Vector2 Accessors
	void GetXY(float& out_x, float& out_y) const;
	const float* GetAsFloatArray() const;
	float* GetAsFloatArray();
	float CalcLength() const;
	float CalcLengthSquared() const;
	float CalcAngleDegrees() const;
	float CalcAngleRadians() const;

	//Vector2 Mutators
	void SetXY(float newX, float newY);
	void Rotate90Degrees();
	void RotateNegative90Degrees();
	void RotateDegrees(float degrees);
	void RotateRadians(float radians);
	void Normalize();
	void SetLength(float newLength);
	void ScaleUniform(float scale);
	void ScaleNonUniform(const Vector2& perAxisScaleFactors);
	void InverseScaleNonUniform(const Vector2& perAxisDivisors);
	void SetUnitLengthAndYawDegrees(float yawDegrees);
	void SetUnitLengthAndYawRadians(float yawRadians);
	void SetLengthAndYawDegrees(float newLength, float yawDegrees);
	void SetLengthAndYawRadians(float newLength, float yawRadians);

	//Vector2 Operators
	bool operator == (const Vector2& vectorToEqual) const;
	bool operator != (const Vector2& vectorToNotEqual) const;
	const Vector2 operator + (const Vector2& vectorToAdd) const;
	const Vector2 operator - (const float scale) const;
	const Vector2 operator - (const Vector2& vectorToSubtract) const;
	const Vector2 operator * (float scale) const;
	const Vector2 operator * (const Vector2& perAxisScaleFactors) const;
	const Vector2 operator / (float inverseScale) const;
	void operator *= (float scale);
	void operator *= (const Vector2& perAxisScaleFactors);
	void operator += (const Vector2& vectorToAdd);
	void operator -= (const Vector2& vectorToSubtract);

	//Vector2 Standalone friend functions
	friend float CalcDistance(const Vector2& positionA, const Vector2& positionB);
	friend float CalcDistanceSquared(const Vector2& posA, const Vector2& posB);
	friend float CalcDistanceBetweenPoints(float pointA, float pointB);
	friend const Vector2 operator * (float scale, const Vector2& vectorToScale);
	friend float DotProduct(const Vector2& a, const Vector2& b);
};