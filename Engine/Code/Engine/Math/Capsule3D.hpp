#pragma once
#include "Engine/Math/Vector3.hpp"

//-----------------------------------------------------------------------------------------------
class Capsule3D
{
public:
	Capsule3D() {}
	~Capsule3D() {}
	Capsule3D(const  Capsule3D& copy);
	explicit Capsule3D(float initialX, float initialY, float initialZ, float endX, float endY, float endZ, float initialRadius);

	void AddPadding(float paddingRadius);
	void Translate(const Vector3& translation);


	const  Capsule3D operator + (const Vector3& translation) const;
	const  Capsule3D operator - (const Vector3& antiTranslation) const;
	void operator += (const Vector3& translation);
	void operator -= (const Vector3& antiTranslation);

	friend const Capsule3D Interpolate(const Capsule3D& start, const  Capsule3D& end, float fractionToEnd);
public:
	Vector3 m_start;
	Vector3 m_end;
	float   m_radius;

};