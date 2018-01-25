#pragma once


//-----------------------------------------------------------------------------------------------
class Vector3
{
public:

	static const Vector3 ZERO;
	Vector3();
	Vector3(float initialX, float initialY, float initalZ);
	Vector3(const Vector3& copy);

	//Vector2 Accessors
	void GetXYZ(float& out_x, float& out_y, float& out_z) const;
	const float* GetAsFloatArray() const;
	float* GetAsFloatArray();
	float CalcLength() const;
	float CalcLengthSquared() const;

	//Vector2 Mutators
	void SetXYZ(float newX, float newY, float newZ);;
	void Normalize();
	void SetLength(float newLength);
	void ScaleUniform(float scale);
	void ScaleNonUniform(const Vector3& perAxisScaleFactors);
	void InverseScaleNonUniform(const Vector3& perAxisDivisors);


	Vector3 Cross(Vector3 const &other) const;
	//Vector2 Operators
	bool operator == (const Vector3& vectorToEqual) const;
	bool operator != (const Vector3& vectorToNotEqual) const;
	const Vector3 operator + (const Vector3& vectorToAdd) const;
	const Vector3 operator - (const Vector3& vectorToSubtract) const;
	const Vector3 operator * (float scale) const;
	const Vector3 operator * (const Vector3& perAxisScaleFactors) const;
	const Vector3 operator / (float inverseScale) const;
	void operator *= (float scale);
	void operator *= (const Vector3& perAxisScaleFactors);
	void operator += (const Vector3& vectorToAdd);
	void operator -= (const Vector3& vectorToSubtract);

	//Vector2 Standalone friend functions
	friend float CalcDistance(const Vector3& positionA, const Vector3& positionB);
	friend float CalcDistanceSquared(const Vector3& posA, const Vector3& posB);
	friend float CalcDistanceBetweenPoints(float pointA, float pointB);
	friend const Vector3 operator * (float scale, const Vector3& vectorToScale);
	friend float DotProduct(const Vector3& a, const Vector3& b);

public:
	float x;
	float y;
	float z;
};