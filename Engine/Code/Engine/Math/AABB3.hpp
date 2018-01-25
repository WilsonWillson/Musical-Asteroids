#pragma once
#include "Engine/Math/Vector3.hpp"

class AABB3
{
public:
	AABB3();
	~AABB3();
	AABB3(const AABB3& copy);
	explicit AABB3(float initialX, float initialY, float initialZ);
	explicit AABB3(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	explicit AABB3(const Vector3& mins, const Vector3& maxs);
	explicit AABB3(const Vector3& center, float radiusX, float radiusY, float radiusZ);

	void StretchToIncludePoint(const Vector3& point);
	void AddPadding(float xPaddingRadius, float yPaddingRadius, float zPaddingRadius);
	void Translate(const Vector3& translation);
	bool IsPointInside(const Vector3& point) const;
	const Vector3 CalcSize() const;
	const Vector3 CalcCenter() const;
	float CalcArea();
	const Vector3 GetPointAtNormalizedPositionWithinBox(const Vector3& normalizedPosition) const;
	const Vector3 GetNormalizedPositionForPointWithinBox(const Vector3& point) const;
	bool DoAABBsOverlap(const AABB3& first, const AABB3& second);

	const AABB3 operator + (const Vector3& translation) const;
	const AABB3 operator - (const Vector3& antiTranslation) const;
	void operator += (const Vector3& translation);
	void operator -= (const Vector3& antiTranslation);

	friend const AABB3 Interpolate(const AABB3& start, const AABB3& end, float fractionToEnd);
public:
	Vector3 m_mins;
	Vector3 m_maxs;
};