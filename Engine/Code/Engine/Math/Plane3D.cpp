#include "Engine/Math/Plane3D.hpp"

Plane3D::Plane3D()
	:m_normal(0.f, 0.f, 0.f),
	m_distToOrigin(0.f)
{

}

Plane3D::~Plane3D()
{

}

bool Plane3D::isPointInsidePlane(const Vector3& point)
{
	float distToPoint = DotProduct(m_normal, point);
	return distToPoint == m_distToOrigin;
}

bool Plane3D::isPointInFrontOfPlane(const Vector3& point)
{
	return DotProduct(m_normal, point) > m_distToOrigin;
}

bool Plane3D::isPointBehindPlane(const Vector3& point)
{
	return DotProduct(m_normal, point) < m_distToOrigin;
}

bool Plane3D::isSphereOverlapingPlane(const Sphere3D& sphere)
{
	float distToPoint = DotProduct(m_normal, sphere.m_center);
	if (distToPoint < m_distToOrigin + sphere.m_radius && distToPoint > m_distToOrigin - sphere.m_radius)
		return true;
	else
		return false;
}

