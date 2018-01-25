#include "Engine/Core/Camera.hpp"


//------------------------------------Constructors/Deconstructors-------------------------------------
Camera::Camera()
:m_yawDegreesAboutY(0.f),
m_rollDegreesAboutZ(0.f),
m_pitchDegreesAboutX(0.f)
{
	m_pos = Vector3(0, 0, 0);
}

Camera::~Camera()
{

}

void Camera::ResetCamera()
{
	m_pos = Vector3(0, 0, 0);
	m_yawDegreesAboutY = 0.f;
	m_rollDegreesAboutZ = 0.f;
	m_pitchDegreesAboutX = 0.f;
}

//---------------------------------Calculate Camera Vectors---------------------------------------------
Vector3 Camera::GetUpXY()
{
	Vector3 upVector(m_forwardMatrix.m_iBasis.y, m_forwardMatrix.m_jBasis.y, m_forwardMatrix.m_kBasis.y);
	return upVector;
}

Vector3 Camera::GetRightXY()
{
	Vector3 rightVector(m_forwardMatrix.m_iBasis.x, m_forwardMatrix.m_jBasis.x, m_forwardMatrix.m_kBasis.x);
	return  rightVector;
}


Vector3 Camera::GetForwardXYZ()
{
	Vector3 forwardVector(m_forwardMatrix.m_iBasis.z, m_forwardMatrix.m_jBasis.z, m_forwardMatrix.m_kBasis.z);
	return  forwardVector;
}

//------------------------------------Getters and Setters--------------------------------------------
void Camera::SetForwards()
{
	Matrix4 pitch;
	pitch.m_jBasis.y = CosDegrees(m_pitchDegreesAboutX);
	pitch.m_jBasis.z = -SinDegrees(m_pitchDegreesAboutX);
	pitch.m_kBasis.y = SinDegrees(m_pitchDegreesAboutX);
	pitch.m_kBasis.z = CosDegrees(m_pitchDegreesAboutX);

	Matrix4 yaw;
	yaw.m_iBasis.x = CosDegrees(m_yawDegreesAboutY);
	yaw.m_iBasis.z = -SinDegrees(m_yawDegreesAboutY);
	yaw.m_kBasis.x = SinDegrees(m_yawDegreesAboutY);
	yaw.m_kBasis.z = CosDegrees(m_yawDegreesAboutY);

	Matrix4 roll;
	roll.m_iBasis.x = CosDegrees(m_rollDegreesAboutZ);
	roll.m_iBasis.y = -SinDegrees(m_rollDegreesAboutZ);
	roll.m_jBasis.x = SinDegrees(m_rollDegreesAboutZ);
	roll.m_jBasis.y = CosDegrees(m_rollDegreesAboutZ);

	Matrix4 indentity;

	indentity = indentity * roll;
	indentity = indentity * pitch;
	indentity = indentity * yaw;

	m_forwardMatrix = indentity;
}

