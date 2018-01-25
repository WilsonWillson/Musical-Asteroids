#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/MathUtils.hpp"

AABB3::AABB3()
	:m_mins(0.f, 0.f, 0.f),
	m_maxs(0.f, 0.f, 0.f)
{

}

AABB3::AABB3(const AABB3& copy)
	:m_mins(copy.m_mins),
	m_maxs(copy.m_maxs)
{

}

AABB3::AABB3(float initialX, float initialY, float initialZ)
	:m_mins(initialX, initialY, initialZ),
	m_maxs(initialX, initialY, initialZ)
{

}

AABB3::AABB3(float minX

	, float minY, float minZ, float maxX, float maxY, float maxZ)
	:m_mins(minX, minY, minZ),
	m_maxs(maxX, maxY, maxZ)
{

}

AABB3::AABB3(const Vector3& mins, const Vector3& maxs)
	:m_mins(mins),
	m_maxs(maxs)
{

}

AABB3::AABB3(const Vector3& center, float radiusX, float radiusY, float radiusZ)
	:m_mins(center.x - radiusX, center.y - radiusY, center.z - radiusZ),
	m_maxs(center.x + radiusX, center.y + radiusY, center.z + radiusZ)
{

}

void AABB3::StretchToIncludePoint(const Vector3& point)
{

	if (m_maxs.x < point.x)
		m_maxs.x = point.x;
	if (m_maxs.y < point.y)
		m_maxs.y = point.y;
	if (m_maxs.z < point.z)
		m_maxs.z = point.z;

	if (m_mins.x > point.x)
		m_mins.x = point.x;
	if (m_mins.y > point.y)
		m_mins.y = point.y;
	if (m_mins.z > point.z)
		m_mins.z = point.z;
}

void AABB3::AddPadding(float xPaddingRadius, float yPaddingRadius, float zPaddingRadius)
{
	m_mins.x -= xPaddingRadius;
	m_mins.y -= yPaddingRadius;
	m_mins.z += zPaddingRadius;
	m_maxs.x += xPaddingRadius;
	m_maxs.y += yPaddingRadius;
	m_maxs.z += zPaddingRadius;
}

void AABB3::Translate(const Vector3& translation)
{
	m_mins += translation;
	m_maxs += translation;
}

bool AABB3::IsPointInside(const Vector3& point) const
{
	if (point.x > m_mins.x &&
		point.y > m_mins.y &&
		point.z > m_mins.z &&
		point.x < m_maxs.x &&
		point.y < m_maxs.y &&
		point.z < m_maxs.z)
		return true;
	return false;
}

const Vector3 AABB3::CalcSize() const
{
	Vector3 size;
	size.x = m_maxs.x - m_mins.x;
	size.y = m_maxs.y - m_mins.y;
	size.z = m_maxs.z - m_mins.z;
	return size;
}

const Vector3 AABB3::CalcCenter() const
{
	Vector3 Center;
	float HalfLength = (m_maxs.x - m_mins.x) / 2;
	float HalfWidth = (m_maxs.y - m_mins.y) / 2;
	float HalfZ = (m_maxs.z - m_mins.z) / 2;
	Center.x += (m_mins.x + HalfLength);
	Center.y += (m_mins.y + HalfWidth);
	Center.z += (m_mins.z + HalfZ);
	return Center;
}

float AABB3::CalcArea()
{
	return (m_maxs.x - m_mins.x) * (m_maxs.y - m_mins.y) * (m_maxs.z - m_mins.z);
}

const Vector3 AABB3::GetPointAtNormalizedPositionWithinBox(const Vector3& normalizedPosition) const
{
	Vector3 normalized;
	normalized.x = RangeMapFloat(normalizedPosition.x, 0.f, 1.f, m_mins.x, m_maxs.x);
	normalized.y = RangeMapFloat(normalizedPosition.y, 0.f, 1.f, m_mins.y, m_maxs.y);
	normalized.z = RangeMapFloat(normalizedPosition.z, 0.f, 1.f, m_mins.z, m_maxs.z);
	return normalized;
}

const Vector3 AABB3::GetNormalizedPositionForPointWithinBox(const Vector3& point) const
{
	Vector3 normalized;
	normalized.x = RangeMapFloat(point.x, m_mins.x, m_maxs.x, 0.f, 1.f);
	normalized.y = RangeMapFloat(point.y, m_mins.y, m_maxs.y, 0.f, 1.f);
	normalized.z = RangeMapFloat(point.z, m_mins.z, m_maxs.z, 0.f, 1.f);
	return normalized;
}

bool AABB3::DoAABBsOverlap(const AABB3& first, const AABB3& second)
{
	if (first.m_mins.y > second.m_maxs.y)
		return false;
	if (second.m_mins.y > first.m_maxs.y)
		return false;
	if (first.m_mins.x > second.m_maxs.x)
		return false;
	if (second.m_mins.x > first.m_maxs.x)
		return false;
	if (first.m_mins.z > second.m_maxs.z)
		return false;
	if (second.m_mins.z > first.m_maxs.z)
		return false;
	return true;
}

void AABB3::operator-=(const Vector3& antiTranslation)
{
	m_mins -= antiTranslation;
	m_maxs -= antiTranslation;
}

void AABB3::operator+=(const Vector3& translation)
{
	m_mins += translation;
	m_maxs += translation;
}

const AABB3 AABB3::operator-(const Vector3& antiTranslation) const
{
	AABB3 newBox;
	newBox.m_maxs = newBox.m_maxs - antiTranslation;
	newBox.m_mins = newBox.m_mins - antiTranslation;
	return newBox;
}

const AABB3 AABB3::operator+(const Vector3& translation) const
{
	AABB3 newBox;
	newBox.m_maxs = newBox.m_maxs + translation;
	newBox.m_mins = newBox.m_mins + translation;
	return newBox;
}

AABB3::~AABB3()
{

}

const AABB3 Interpolate(const AABB3& start, const AABB3& end, float fractionToEnd)
{
	AABB3 blended;
	float fractionOfStart = fractionToEnd - 1.f;
	blended.m_mins = (fractionOfStart * start.m_mins) + (fractionToEnd * end.m_mins);
	blended.m_maxs = (fractionOfStart * start.m_maxs) + (fractionToEnd * end.m_maxs);
	return blended;
}