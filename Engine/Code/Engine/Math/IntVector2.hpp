#pragma once

class IntVector2
{
public:
	IntVector2();
	IntVector2(int initialX, int initialY);
	IntVector2(const IntVector2& copy);
	~IntVector2();

	//Vector2 Accessors
	void GetXY(int& out_x, int& out_y) const;
	const int* GetAsIntArray() const;
	int* GetAsIntArray();
	int CalcLength() const;
	int CalcLengthSquared() const;
	int CalcAngleDegrees() const;
	int CalcAngleRadians() const;

	//Vector2 Mutators
	void SetXY(int newX, int newY);
	void Rotate90Degrees();
	void RotateNegative90Degrees();
	void RotateDegrees(int degrees);
	void RotateRadians(int radians);
	void Normalize();
	void SetLength(int newLength);
	void ScaleUniform(int scale);
	void ScaleNonUniform(const IntVector2& perAxisScaleFactors);
	void InverseScaleNonUniform(const IntVector2& perAxisDivisors);
	void SetUnitLengthAndYawDegrees(int yawDegrees);
	void SetUnitLengthAndYawRadians(int yawRadians);
	void SetLengthAndYawDegrees(int newLength, int yawDegrees);
	void SetLengthAndYawRadians(int newLength, int yawRadians);

	//Vector2 Operators
	bool operator == (const IntVector2& vectorToEqual) const;
	bool operator != (const IntVector2& vectorToNotEqual) const;
	bool operator < (const IntVector2& rhs) const;
	const IntVector2 operator + (const IntVector2& vectorToAdd) const;
	const IntVector2 operator - (const IntVector2& vectorToSubtract) const;
	const IntVector2 operator * (int scale) const;
	const IntVector2 operator * (const IntVector2& perAxisScaleFactors) const;
	const IntVector2 operator / (int inverseScale) const;
	void operator *= (int scale);
	void operator *= (const IntVector2& perAxisScaleFactors);
	void operator += (const IntVector2& vectorToAdd);
	void operator -= (const IntVector2& vectorToSubtract);

	//Vector2 Standalone friend functions
	friend int CalcDistance(const IntVector2& positionA, const IntVector2& positionB);
	friend int CalcDistanceSquared(const IntVector2& posA, const IntVector2& posB);
	friend const IntVector2 operator * (int scale, const IntVector2& vectorToScale);
	friend int DotProduct(const IntVector2& a, const IntVector2& b);

public:
	int x;
	int y;
	static const IntVector2 ZERO;
};