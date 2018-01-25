#pragma once
#include "Engine/Math/Vector3.hpp"

//-----------------------------------------------------------------------------------------------
class LineSegment3D
{
public:
	LineSegment3D();
	~LineSegment3D();
	LineSegment3D(const LineSegment3D& copy);

	void AddPadding(float padding);
	void Translate(const Vector3& translation);


	const Vector3 CalcCenter() const;
	float  CalcSlope() const;
	Vector3 GetPointAtParametric(float time);

	const LineSegment3D operator + (const Vector3& translation) const;
	const LineSegment3D operator - (const Vector3& antiTranslation) const;
	void operator += (const Vector3& translation); 
	void operator -= (const Vector3& antiTranslation);

	friend const LineSegment3D Interpolate(const LineSegment3D& start, const LineSegment3D& end, float fractionToEnd);
public:
	Vector3 m_start;
	Vector3 m_end;

};