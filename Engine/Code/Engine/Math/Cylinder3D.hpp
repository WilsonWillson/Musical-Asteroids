#pragma once
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Plane3D.hpp"
#include <vector>
//-----------------------------------------------------------------------------------------------
class Cylinder3D
{
public:

	Cylinder3D();
	~Cylinder3D();
	Cylinder3D(const  Cylinder3D& copy);
	explicit Cylinder3D(Vector3 start, Vector3 end, Vector3 radius);

	void Translate(const Vector3& translation);

public:
	Vector3 m_start;
	Vector3 m_end;
	Vector3 m_radius;
};