#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/MathUtils.hpp"
const AABB2 AABB2::ZERO_TO_ONE(0.f, 1.f);

AABB2::AABB2()
	:m_mins(0.f, 0.f),
	m_maxs(0.f, 0.f)
{

}

AABB2::AABB2(const AABB2& copy)
	:m_mins(copy.m_mins),
	m_maxs(copy.m_maxs)
{

}

AABB2::AABB2(const Vector2& center, float radiusX, float radiusY)
	:m_mins(center.x - radiusX, center.y - radiusY),
	m_maxs(center.x + radiusX, center.y + radiusY)
{

}

AABB2::AABB2(float minX, float minY, float maxX, float maxY)
	:m_mins(minX, minY),
	m_maxs(maxX, maxY)
{

}

AABB2::AABB2(float initialX, float initialY)
	:m_mins(initialX, initialY),
	m_maxs(initialX, initialY)
{

}

AABB2::AABB2(const Vector2& mins, const Vector2& maxs)
	:m_mins(mins),
	m_maxs(maxs)
{

}

AABB2::~AABB2()
{

}

void AABB2::StretchToIncludePoint(const Vector2& point)
{
	if (m_maxs.x < point.x)
		m_maxs.x = point.x;
	if (m_maxs.y < point.y)
		m_maxs.y = point.y;
	if (m_mins.x < point.x)
		m_mins.x = point.x;
	if (m_mins.y < point.y)
		m_mins.y = point.y;
}

void AABB2::AddPadding(float xPaddingRadius, float yPaddingRadius)
{
	m_mins.x -= xPaddingRadius;
	m_mins.y -= yPaddingRadius;
	m_maxs.x += xPaddingRadius;
	m_maxs.y += yPaddingRadius;
}

void AABB2::Translate(const Vector2& translation)
{
	m_mins += translation;
	m_maxs += translation;
}

bool AABB2::IsPointInside(const Vector2& point) const
{
	if (point.x > m_mins.x &&
		point.y > m_mins.y &&
		point.x < m_maxs.x &&
		point.y > m_maxs.y)
		return true;
	return false;
}

float AABB2::CalcArea() {
	return (m_maxs.x - m_mins.x) * (m_maxs.y - m_mins.y);
}


const Vector2 AABB2::CalcSize() const
{
	Vector2 size;
	size.x = m_maxs.x - m_mins.x;
	size.y = m_maxs.y - m_mins.y;
	return size;
}

const Vector2 AABB2::CalcCenter() const
{
	Vector2 Center;
	float HalfLength = (m_maxs.x - m_mins.x)/2;
	float HalfWidth = (m_maxs.y - m_mins.y)/2;
	Center.x += (m_mins.x + HalfLength);
	Center.y += (m_mins.y + HalfWidth);
	return Center;
}

const Vector2 AABB2::GetPointAtNormalizedPositionWithinBox(const Vector2& normalizedPosition) const
{
	Vector2 normalized;
	normalized.x = RangeMapFloat(normalizedPosition.x, 0.f, 1.f, m_mins.x, m_maxs.x);
	normalized.y = RangeMapFloat(normalizedPosition.y, 0.f, 1.f, m_mins.y, m_maxs.y);
	return normalized;
}

//--------------------------------------------------------------------------------------------------
const Vector2 AABB2::GetNormalizedPositionForPointWithinBox(const Vector2& point) const
{
	Vector2 normalized;
	normalized.x = RangeMapFloat(point.x, m_mins.x, m_maxs.x, 0.f, 1.f);
	normalized.y = RangeMapFloat(point.y, m_mins.y, m_maxs.y, 0.f, 1.f);
	return normalized;
}



bool AABB2::DoAABBsOverlap(const AABB2& first, const AABB2& second)
{
	if (first.m_mins.y > second.m_maxs.y)
		return false;
	if (second.m_mins.y > first.m_maxs.y)
		return false;
	if (first.m_mins.x > second.m_maxs.x)
		return false;
	if (second.m_mins.x > first.m_maxs.x)
		return false;
	return true;
}

Vector2 AABB2::GetPositionFromNormalizedPosition(Vector2 vector)
{
	Vector2 newPosition;
	newPosition.x = RangeMapFloat(vector.x, 0, 1, m_mins.x, m_maxs.x);
	newPosition.y = RangeMapFloat(vector.y, 0, 1, m_mins.y, m_maxs.y);
	return newPosition;
}

Vector2 AABB2::get_uv(Vector2 cords)
{
	Vector2 newPosition;
	newPosition.x = RangeMapFloat(cords.x, m_mins.x, m_maxs.x, 0, 1);
	newPosition.y = RangeMapFloat(cords.y, m_mins.y, m_maxs.y, 0, 1);
	return newPosition;
}

void AABB2::operator-=(const Vector2& antiTranslation)
{
	m_mins -= antiTranslation;
	m_maxs -= antiTranslation;
}

void AABB2::operator+=(const Vector2& translation)
{
	m_mins += translation;
	m_maxs += translation;
}

const AABB2 AABB2::operator-(const Vector2& antiTranslation) const
{
	AABB2 newBox;
	newBox.m_maxs = newBox.m_maxs - antiTranslation;
	newBox.m_mins = newBox.m_mins - antiTranslation;
	return newBox;
}

const AABB2 AABB2::operator+(const Vector2& translation) const
{
	AABB2 newBox;
	newBox.m_maxs = newBox.m_maxs + translation;
	newBox.m_mins = newBox.m_mins + translation;
	return newBox;
}


 const AABB2 Interpolate(const AABB2& start, const AABB2& end, float fractionToEnd)
{
	AABB2 blended;
	float fractionOfStart = fractionToEnd - 1.f;
	blended.m_mins = (fractionOfStart * start.m_mins) + (fractionToEnd * end.m_mins);
	blended.m_maxs = (fractionOfStart * start.m_maxs) + (fractionToEnd * end.m_maxs);
	return blended;
}


