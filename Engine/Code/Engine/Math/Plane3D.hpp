#pragma once
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Sphere3D.hpp"
//-----------------------------------------------------------------------------------------------
class Plane3D
{
public:

	Plane3D();
	~Plane3D();
	bool isPointInsidePlane(const Vector3& point);
	bool isPointInFrontOfPlane(const Vector3& point);
	bool isPointBehindPlane(const Vector3& point);

	bool isSphereOverlapingPlane(const Sphere3D& sphere);

public:
	Vector3 m_normal;
	float m_distToOrigin;

};