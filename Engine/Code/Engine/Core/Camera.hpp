#pragma once
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Matrix4.hpp"
#include "Engine/Math/MathUtils.hpp"

class Camera
{
public:
	Camera();
	~Camera();

	void ResetCamera();

	Vector3 GetUpXY();
	Vector3 GetRightXY();
	Vector3 GetForwardXYZ();

	void SetForwards();

public:
	Matrix4		m_forwardMatrix;
	Vector3		m_pos;
	float		m_yawDegreesAboutY;
	float		m_pitchDegreesAboutX;
	float		m_rollDegreesAboutZ;
};