#pragma once
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/AABB3.hpp"

//-----------------------------------------------------------------------------------------------
class Sphere3D
{
public:
	Sphere3D() {}
	~Sphere3D() {}
	Sphere3D(const  Sphere3D& copy);
	explicit Sphere3D(float initialX, float initialY, float initialZ, float initialRadius);
	explicit Sphere3D(const Vector3& initialCenter, float initialRadius);

	void StretchToIncludePoint(const Vector3& point);
	void AddPadding(float paddingRadius);
	void Translate(const Vector3& translation);

	bool IsPointInside(const Vector3& point) const;
	bool DoDiscsOverlap(const  Sphere3D* first, const  Sphere3D* second);

	const  Sphere3D operator + (const Vector3& translation) const;
	const  Sphere3D operator - (const Vector3& antiTranslation) const;
	void operator += (const Vector3& translation);
	void operator -= (const Vector3& antiTranslation);

	friend const Sphere3D  Interpolate(const  Sphere3D& start, const  Sphere3D& end, float fractionToEnd);
public:
	Vector3 m_center;
	float	m_radius;

};