#include "Engine/Math/Disc2D.hpp"
#include <math.h>
static const Disc2D UNIT_CIRCLE;

Disc2D::Disc2D(const Disc2D& copy)
	:m_center(copy.m_center.x, copy.m_center.y),
	m_radius(copy.m_radius)
{
	
}

Disc2D::Disc2D(float initialX, float initialY, float initialRadius)
	:m_center(initialX, initialY),
	m_radius(initialRadius)
{

}

Disc2D::Disc2D(const Vector2& initialCenter, float initialRadius)
	:m_center(initialCenter),
	m_radius(initialRadius)
{

}
//-----------------------------------------------------------------------------
void Disc2D::StretchToIncludePoint(const Vector2& point)
{
	float distToPoint = CalcDistance(point, m_center);
	m_radius = distToPoint;
}

void Disc2D::AddPadding(float paddingRadius)
{
	m_radius += paddingRadius;
}

void Disc2D::Translate(const Vector2& translation)
{
	m_center += translation;
}

bool Disc2D::IsPointInside(const Vector2& point) const
{
	float distToPoint = CalcDistance(point, m_center);
	if (distToPoint < m_radius)
		return true;
	else
		return false;
}

bool Disc2D::DoDiscsandAABBOverlap(const Disc2D* first, const AABB2* second)
{
	float distToPointX = CalcDistanceBetweenPoints(first->m_center.x, second->m_maxs.x);
	if (fabs(distToPointX) < m_radius)
		return true;

	distToPointX = CalcDistanceBetweenPoints(first->m_center.x, second->m_mins.x);
	if (fabs(distToPointX) < m_radius)
		return true;
	/*
	float distToPointY = CalcDistanceBetweenPoints(first->m_center.y, second->m_maxs.y);
	if (fabs(distToPointY) < m_radius)
		return true;

	distToPointY = CalcDistanceBetweenPoints(first->m_center.y, second->m_maxs.y);
	if (fabs(distToPointY) < m_radius )
		return true;
*/
	return false;
}

float Disc2D::HowMuchDoesDiscOverlapAABB(const Disc2D* first, const AABB2* second)
{
	float distToPoint = CalcDistanceBetweenPoints(first->m_center.x, second->m_maxs.x);
	if (fabs(distToPoint) < m_radius)
		return (second->m_maxs.x - m_center.x) * .1f;

	distToPoint = CalcDistanceBetweenPoints(first->m_center.x, second->m_mins.x);
	if (fabs(distToPoint) < m_radius)
		return (second->m_mins.x - m_center.x) * .1f;
	/*
	distToPoint = CalcDistanceBetweenPoints(first->m_center.y, second->m_maxs.y);
	if (fabs(distToPoint) < m_radius)
		return distToPoint;

	distToPoint = CalcDistanceBetweenPoints(first->m_center.y, second->m_maxs.y);
	if (fabs(distToPoint) < m_radius)
		return distToPoint;
		*/
	return 0;
}

bool Disc2D::DoDiscsOverlap(const Disc2D* first, const Disc2D* second)
{
	float distToPoint = CalcDistance(first->m_center, second->m_center);
	if (distToPoint <= (first->m_radius + second->m_radius))
		return true;
	return false;
}

bool Disc2D::DoDiscsAndLineSegment2DOverlap(const Disc2D* first, const LineSegment2D* second)
{
	//End Points
	float distToPoint = CalcDistance(first->m_center, second->m_start);
	if (distToPoint <= (first->m_radius))
		return true;

	distToPoint = CalcDistance(first->m_center, second->m_end);
	if (distToPoint <= (first->m_radius))
		return true;

	//MiddlePoint

	return false;
}

const Disc2D Interpolate(const Disc2D& start, const Disc2D& end, float fractionToEnd)
{
	Disc2D blended;
	float fractionOfStart = fractionToEnd - 1.f;
	blended.m_center = (fractionOfStart * start.m_center) + (fractionToEnd * end.m_center);
	blended.m_radius = (fractionOfStart * start.m_radius) + (fractionToEnd * end.m_radius);
	return blended;
}

/*
Vector2 Disc2D::HowMuchDoesDiscOverlapLineSegment2D(const Disc2D* first, const LineSegment2D* second)
{
	//End Points
	float distToPoint = CalcDistance(first->m_center, second->m_start);
	if (distToPoint <= (first->m_radius)) {
		Vector2 displacement = first->m_center - second->m_start;
		displacement.Normalize();
		Vector2 overlapDepth = (first->m_radius + second->m_start) - displacement;
		Vector2 push = displacement * (overlapDepth);
		return push;
	}

	distToPoint = CalcDistance(first->m_center, second->m_end);
	if (distToPoint <= (first->m_radius)) {
		Vector2 displacement = first->m_center - second->m_end;
		displacement.Normalize();
		Vector2 overlapDepth = (first->m_radius + second->m_end) - displacement;
		return overlapDepth;
	}
}
*/
void Disc2D::operator-=(const Vector2& antiTranslation)
{
	Disc2D newDisc;
	newDisc.m_center -= antiTranslation;
}

void Disc2D::operator+=(const Vector2& translation)
{
	Disc2D newDisc;
	newDisc.m_center += translation;
}

const Disc2D Disc2D::operator-(const Vector2& antiTranslation) const
{
	Disc2D newDisc;
	newDisc.m_center = newDisc.m_center - antiTranslation;
	return newDisc;
}

const Disc2D Disc2D::operator+(const Vector2& translation) const
{
	Disc2D newDisc;
	newDisc.m_center = newDisc.m_center + translation;
	return newDisc;
}
