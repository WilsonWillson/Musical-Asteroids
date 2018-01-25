#pragma once
#include "Engine/Math/Vector2.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////
class AABB2
{
public:
	AABB2();
	~AABB2();
	AABB2(const AABB2& copy);
	explicit AABB2(float initialX, float initialY);
	explicit AABB2(float minX, float minY, float maxX, float maxY);
	explicit AABB2(const Vector2& mins, const Vector2& maxs);
	explicit AABB2(const Vector2& center, float radiusX, float radiusY);

	void StretchToIncludePoint(const Vector2& point);
	void AddPadding(float xPaddingRadius, float yPaddingRadius);
	void Translate(const Vector2& translation);
	bool IsPointInside(const Vector2& point) const;
	const Vector2 CalcSize() const;
	const Vector2 CalcCenter() const;
	float CalcArea();
	const Vector2 GetPointAtNormalizedPositionWithinBox(const Vector2& normalizedPosition) const;
	const Vector2 GetNormalizedPositionForPointWithinBox(const Vector2& point) const; 
	bool DoAABBsOverlap(const AABB2& first, const AABB2& second);

	Vector2 GetPositionFromNormalizedPosition(Vector2 vector);
	Vector2 get_uv(Vector2 cords);


	const AABB2 operator + (const Vector2& translation) const;
	const AABB2 operator - (const Vector2& antiTranslation) const;
	void operator += (const Vector2& translation);
	void operator -= (const Vector2& antiTranslation);

	friend const AABB2 Interpolate(const AABB2& start, const AABB2& end, float fractionToEnd);
public:
	Vector2 m_mins;
	Vector2 m_maxs;


	static const AABB2 ZERO_TO_ONE;
};