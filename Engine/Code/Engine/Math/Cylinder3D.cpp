#include "Engine/Math/Cylinder3D.hpp"
Cylinder3D::Cylinder3D()
{

}

Cylinder3D::Cylinder3D(const Cylinder3D& copy)
	: m_start(copy.m_start)
	, m_end(copy.m_end)
	, m_radius(copy.m_radius)
{

}


Cylinder3D::Cylinder3D(Vector3 start, Vector3 end, Vector3 radius)
	: m_start(start)
	, m_end(end)
	, m_radius(radius)
{

}

Cylinder3D::~Cylinder3D()
{

}

void Cylinder3D::Translate(const Vector3& translation)
{
	m_start += translation;
	m_end += translation;
}

