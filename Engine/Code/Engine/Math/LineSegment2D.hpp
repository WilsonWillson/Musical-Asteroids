#pragma once
#include "Engine/Math/Vector2.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////
class LineSegment2D
{
public:
	LineSegment2D();
	~LineSegment2D();
	LineSegment2D(const LineSegment2D& copy);

	void AddPadding(float padding);
	void Translate(const Vector2& translation);
	void StretchToIncludePoint(const Vector2& point);

	const Vector2 CalcCenter() const;
	float  CalcSlope() const;
	Vector2 GetPointAtParametric(float time);

	const LineSegment2D operator + (const Vector2& translation) const;
	const LineSegment2D operator - (const Vector2& antiTranslation) const;
	void operator += (const Vector2& translation);
	void operator -= (const Vector2& antiTranslation);

	friend const LineSegment2D Interpolate(const  LineSegment2D& start, const  LineSegment2D& end, float fractionToEnd);
public:
	Vector2 m_start;
	Vector2 m_end;
};