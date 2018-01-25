#pragma once
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Plane3D.hpp"
#include <vector>
//-----------------------------------------------------------------------------------------------
class ConvexHull3D
{
public:

	ConvexHull3D();
	~ConvexHull3D();

public:
	std::vector<Plane3D> m_boundingPlanes;
};