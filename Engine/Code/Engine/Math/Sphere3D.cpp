#include "Engine/Math/Sphere3D.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/AABB3.hpp"

Sphere3D::Sphere3D(float initialX, float initialY, float initialZ, float initialRadius)
	:m_center(initialX, initialY, initialZ),
	m_radius(initialRadius)
{

}

Sphere3D::Sphere3D(const Vector3& initialCenter, float initialRadius)
	:m_center(initialCenter),
	m_radius(initialRadius)
{

}

Sphere3D::Sphere3D(const Sphere3D& copy)
	:m_center(copy.m_center.x, copy.m_center.y, copy.m_center.z),
	m_radius(copy.m_radius)
{

}

void Sphere3D::StretchToIncludePoint(const Vector3& point)
{
	float distToPoint = CalcDistance(point, m_center);
	m_radius = distToPoint;
}

void Sphere3D::AddPadding(float paddingRadius)
{
	m_radius += paddingRadius;
}

void Sphere3D::Translate(const Vector3& translation)
{
	m_center += translation;
}

bool Sphere3D::IsPointInside(const Vector3& point) const
{
	float distToPoint = CalcDistance(point, m_center);
	if (distToPoint < m_radius)
		return true;
	else
		return false;
}

bool Sphere3D::DoDiscsOverlap(const Sphere3D* first, const Sphere3D* second)
{
	float distToPoint = CalcDistance(first->m_center, second->m_center);
	if (distToPoint <= (first->m_radius + second->m_radius))
		return true;
	return false;
}

void Sphere3D::operator-=(const Vector3& antiTranslation)
{
	Sphere3D newDisc;
	newDisc.m_center -= antiTranslation;
}

void Sphere3D::operator+=(const Vector3& translation)
{
	Sphere3D newDisc;
	newDisc.m_center += translation;
}

const Sphere3D Sphere3D::operator-(const Vector3& antiTranslation) const
{
	Sphere3D newDisc;
	newDisc.m_center = newDisc.m_center - antiTranslation;
	return newDisc;
}

const Sphere3D Sphere3D::operator+(const Vector3& translation) const
{
	Sphere3D newDisc;
	newDisc.m_center = newDisc.m_center + translation;
	return newDisc;
}

const Sphere3D Interpolate(const Sphere3D& start, const Sphere3D& end, float fractionToEnd)
{
	Sphere3D  blended;
	float fractionOfStart = fractionToEnd - 1.f;
	blended.m_center = (fractionOfStart * start.m_center) + (fractionToEnd * end.m_center);
	blended.m_radius = (fractionOfStart * start.m_radius) + (fractionToEnd * end.m_radius);
	return blended;
}
