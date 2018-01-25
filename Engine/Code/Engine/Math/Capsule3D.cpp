#include "Engine/Math/Capsule3D.hpp"
#include "Engine/Math/Vector3.hpp"

Capsule3D::Capsule3D(const Capsule3D& copy)
	: m_start(copy.m_start)
	, m_end(copy.m_end)
	, m_radius(copy.m_radius)
{

}

Capsule3D::Capsule3D(float initialX, float initialY, float initialZ, float endX, float endY, float endZ, float initialRadius) 
	: m_start(initialX, initialY, initialZ)
	, m_end(endX, endY, endZ)
	, m_radius(initialRadius)
{

}

void Capsule3D::AddPadding(float padding)
{
	m_radius += padding;
	m_start.x -= padding;
	m_start.y -= padding;
	m_start.z -= padding;
	m_end.x += padding;
	m_end.y += padding;
	m_end.z += padding;
}

void Capsule3D::Translate(const Vector3& translation)
{
	m_start += translation;
	m_end += translation;
}

const Capsule3D Interpolate(const Capsule3D& start, const Capsule3D& end, float fractionToEnd)
{
	Capsule3D  blended;
	float fractionOfStart = fractionToEnd - 1.f;
	blended.m_start = (fractionOfStart * start.m_start) + (fractionOfStart * end.m_start);
	blended.m_end = (fractionOfStart * start.m_end) + (fractionOfStart * end.m_end);
	blended.m_radius = (fractionOfStart * start.m_radius) + (fractionToEnd * end.m_radius);
	return blended;

}

void Capsule3D::operator-=(const Vector3& antiTranslation)
{
	m_start -= antiTranslation;
	m_end -= antiTranslation;
}

void Capsule3D::operator+=(const Vector3& translation)
{
	m_start += translation;
	m_end += translation;
}

const Capsule3D Capsule3D::operator-(const Vector3& antiTranslation) const
{
	Capsule3D  newCapsule;
	newCapsule.m_start = newCapsule.m_start - antiTranslation;
	return newCapsule;
}

const Capsule3D Capsule3D::operator+(const Vector3& translation) const
{
	Capsule3D  newCapsule;
	newCapsule.m_start = newCapsule.m_start - translation;
	newCapsule.m_end = newCapsule.m_end - translation;
	return newCapsule;
}

