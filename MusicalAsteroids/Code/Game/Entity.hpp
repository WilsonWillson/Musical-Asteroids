#pragma once
#include "Engine/Math/Vector2.hpp"



class Entity
{
public:
	Entity();
	Entity(Vector2 position, Vector2 velocity, float spinDegreePerSecond, float orientatioDegree, float radius, float physicsRadius, int health);
	~Entity();


	void DetermineWrapping();

public:
	Vector2			m_position;
	Vector2			m_velocity;
	float			m_spinDegreesPerSecond;
	float			m_orientationDegrees;
	float			m_radius;
	float			m_PhysicsRadius;
	int				m_health;
	bool	m_wrappingHasHappened;
};
