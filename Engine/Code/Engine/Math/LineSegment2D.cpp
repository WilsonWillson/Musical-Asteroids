#include "Engine/Math/LineSegment2D.hpp"
#include "Engine/Math/Vector2.hpp"
LineSegment2D::LineSegment2D()
	: m_start(0.f,0.f)
	, m_end(0.f,0.f)
{

}

LineSegment2D::LineSegment2D(const LineSegment2D& copy)
	: m_start(copy.m_start)
	, m_end(copy.m_end)
{

}

void LineSegment2D::AddPadding(float padding)
{
	m_start.x -= padding;
	m_start.y -= padding;
	m_end.x += padding;
	m_end.y += padding;
}

void LineSegment2D::Translate(const Vector2& translation)
{
	m_start += translation;
	m_end += translation;
}

void LineSegment2D::StretchToIncludePoint(const Vector2& point)
{
	if (m_start.x < point.x)
		m_start.x = point.x;
	if (m_start.y < point.y)
		m_start.y = point.y;
	if (m_end.x < point.x)
		m_end.x = point.x;
	if (m_end.y < point.y)
		m_end.y = point.y;
}


const Vector2 LineSegment2D::CalcCenter() const
{
	Vector2 center;
	center.x = (m_end.x - m_start.x);
	center.y = (m_end.y - m_start.y);
	return center;
}

float LineSegment2D::CalcSlope() const
{
	float rise = (m_end.y - m_start.y);
	float run = (m_end.x - m_start.x);
	return (rise / run);
}

Vector2 LineSegment2D::GetPointAtParametric(float time)
{
	Vector2 point = m_start + time * (m_end - m_start);
	return point;
}

LineSegment2D::~LineSegment2D()
{

}

void LineSegment2D ::operator-=(const Vector2& antiTranslation)
{
	LineSegment2D  newLine;
	newLine.m_start -= antiTranslation;
	newLine.m_end -= antiTranslation;
}

void LineSegment2D ::operator+=(const Vector2& translation)
{
	LineSegment2D  newLine;
	newLine.m_start -= translation;
	newLine.m_end -= translation;
}

const LineSegment2D  LineSegment2D ::operator-(const Vector2& antiTranslation) const
{
	LineSegment2D  newLine;
	newLine.m_start = newLine.m_start - antiTranslation;
	return newLine;
}

const LineSegment2D  LineSegment2D ::operator+(const Vector2& translation) const
{
	LineSegment2D  newLine;
	newLine.m_start = newLine.m_start - translation;
	newLine.m_end = newLine.m_end - translation;
	return newLine;
}

const LineSegment2D Interpolate(const LineSegment2D& start, const LineSegment2D& end, float fractionToEnd)
{
	LineSegment2D  blended;
	float fractionOfStart = fractionToEnd - 1.f;
	blended.m_start = (fractionOfStart * start.m_start) + (fractionOfStart * end.m_start);
	blended.m_end = (fractionOfStart * start.m_end) + (fractionOfStart * end.m_end);
	return blended;
}
