#include "Game/Entity.hpp"


//-----------------------Consturtor/Deconstructor-------------------------------
Entity::Entity()
	: m_position(0.f, 0.f)
	, m_velocity(0.f, 0.f)
	, m_spinDegreesPerSecond(0.f)
	, m_orientationDegrees(0.f)
	, m_radius(0.f)
	, m_PhysicsRadius(0.f)
	, m_health(0)
{
}

Entity::Entity(Vector2 position, Vector2 velocity, float spinDegreePerSecond, float orientatioDegree, float radius, float physicsRadius, int health)
	: m_position(position)
	, m_velocity(velocity)
	, m_spinDegreesPerSecond(spinDegreePerSecond)
	, m_orientationDegrees(orientatioDegree)
	, m_radius(radius)
	, m_PhysicsRadius(physicsRadius)
	, m_health(health)
{
}


Entity::~Entity()
{

}

void Entity::DetermineWrapping()
{
	if (m_position.x > (1600.f + m_radius)) {
		m_position.x = 0.f - m_radius;
		m_wrappingHasHappened = true;
	}
	if (m_position.x < (0.f - m_radius)) {
		m_position.x = 1600.f + m_radius;
		m_wrappingHasHappened = true;
	}
	if (m_position.y > (900.f + m_radius)) {
		m_position.y = 0.f - m_radius;
		m_wrappingHasHappened = true;
	}
	if (m_position.y < (0.f - m_radius)) {
		m_position.y = 900.f + m_radius;
		m_wrappingHasHappened = true;
	}
}