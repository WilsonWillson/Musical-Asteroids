#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/LineSegment3D.hpp"


LineSegment3D::LineSegment3D()
	: m_start(0.f, 0.f, 0.f)
	, m_end(0.f, 0.f, 0.f)
{

}

LineSegment3D::LineSegment3D(const LineSegment3D& copy)
	: m_start(copy.m_start)
	, m_end(copy.m_end)
{

}

void LineSegment3D::AddPadding(float padding)
{
	m_start.x -= padding;
	m_start.y -= padding;
	m_start.z -= padding;
	m_end.x += padding;
	m_end.y += padding;
	m_end.z += padding;
}

void LineSegment3D::Translate(const Vector3& translation)
{
	m_start += translation;
	m_end += translation;
}

const Vector3 LineSegment3D::CalcCenter() const
{
	Vector3 center;
	center.x = (m_end.x - m_start.x);
	center.y = (m_end.y - m_start.y);
	center.y = (m_end.z - m_start.z);
	return center;
}

float LineSegment3D::CalcSlope() const
{
	float rise = m_end.z - m_start.z;
	float run = (m_end.y - m_start.y) * (m_end.x - m_start.x);
	return (rise/run);
}

Vector3 LineSegment3D::GetPointAtParametric(float time)
{
	Vector3 point = m_start + time * (m_end - m_start);
	return point;
}	

void LineSegment3D::operator-=(const Vector3& antiTranslation)
{

	m_start -= antiTranslation;
	m_end -= antiTranslation;
}

void LineSegment3D::operator+=(const Vector3& translation)
{
	m_start += translation;
	m_end += translation;
}

const LineSegment3D LineSegment3D::operator-(const Vector3& antiTranslation) const
{
	LineSegment3D  newLine;
	newLine.m_start = newLine.m_start - antiTranslation;
	return newLine;
}

const LineSegment3D LineSegment3D::operator+(const Vector3& translation) const
{
	LineSegment3D  newLine;
	newLine.m_start = newLine.m_start - translation;
	newLine.m_end = newLine.m_end - translation;
	return newLine;
}

LineSegment3D::~LineSegment3D()
{

}

const LineSegment3D Interpolate(const LineSegment3D& start, const LineSegment3D& end, float fractionToEnd)
{
	LineSegment3D  blended;
	float fractionOfStart = fractionToEnd - 1.f;
	blended.m_start = (fractionOfStart * start.m_start) + (fractionOfStart * end.m_start);
	blended.m_end = (fractionOfStart * start.m_end) + (fractionOfStart * end.m_end);
	return blended;
}
