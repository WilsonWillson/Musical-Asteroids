#pragma once
#include "Engine/Math/Vector3.hpp"

//-----------------------------------------------------------------------------------------------
class Vector4
{
public:

	static const  Vector4 ZERO;
	Vector4();
	Vector4(float initialX, float initialY, float initalZ, float initalW);
	Vector4(Vector3 xyzCords, float initalW);
	Vector4(const  Vector4& copy);

	//Vector4 Accessors
	void GetXYZ(float& out_x, float& out_y, float& out_z);
	void GetXYZW(float& out_x, float& out_y, float& out_z, float& out_w) const;
	const float* GetAsFloatArray() const;
	float* GetAsFloatArray();
	float CalcLength() const;
	float CalcLengthSquared() const;

	//Vector4 Mutators
	void SetXYZ(float newX, float newY, float newZ,float newW);
	void Normalize3D();
	void Normalize4D();
	void SetLength(float newLength);
	void ScaleUniform(float scale);
	void ScaleNonUniform(const Vector4& perAxisScaleFactors);
	void InverseScaleNonUniform(const Vector4& perAxisDivisors);

	//Vector 4 HelperFunctions
	Vector3 xyz() const;


	//Vector 4 Operators
	bool operator == (const Vector4& vectorToEqual) const;
	bool operator != (const Vector4& vectorToNotEqual) const;
	const Vector4 operator + (const Vector4& vectorToAdd) const;
	const Vector4 operator - (const Vector4& vectorToSubtract) const;
	const Vector4 operator * (float scale) const;
	const Vector4 operator * (const Vector4& perAxisScaleFactors) const;
	const Vector4 operator / (float inverseScale) const;
	void operator *= (float scale);
	void operator *= (const Vector4& perAxisScaleFactors);
	void operator += (const Vector4& vectorToAdd);
	void operator -= (const Vector4& vectorToSubtract);

	//Vector4 Standalone friend functions
	friend float CalcDistance(const Vector4& positionA, const Vector4& positionB);
	friend float CalcDistanceSquared(const Vector4& posA, const Vector4& posB);
	friend float CalcDistanceBetweenPoints(float pointA, float pointB);
	friend const Vector4 operator * (float scale, const Vector4& vectorToScale);
	friend float DotProduct(const Vector4& a, const Vector4& b);

public:
	float x;
	float y;
	float z;
	float w;
};
