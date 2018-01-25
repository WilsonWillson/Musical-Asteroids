#include "Game/Bullet.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/GameCommon.hpp"

//---------------------------Constructor/Deconstructor-----------------------------------------------

Bullet::Bullet()
{

}


Bullet::Bullet(Entity bulletStats)
	: Entity(bulletStats),
	m_deathTime(180)
{

}

Bullet::~Bullet()
{

}

//-----------------------------Update------------------------------------------------------------------
void Bullet::Update(float deltaSeconds)
{
	--m_deathTime;
	m_position.x += (m_velocity.x * deltaSeconds);
	m_position.y += (m_velocity.y * deltaSeconds);
	DetermineWrapping();
}

//---------------------------Render-------------------------------------------------------------------
void Bullet::Render()
{
	g_renderSystem->PushMatrix();
	g_renderSystem->Translatef(m_position.x, m_position.y, 0.f);
	g_renderSystem->Rotatef(m_orientationDegrees, 0.f, 0.f, 1.f);
	g_renderSystem->Scalef(1.f, 1.f, 1.f);
	g_renderSystem->BeginPolygon();
	{
		//The Body of the ship
		g_renderSystem->Color3f(.5f, .5f, .5f);
		g_renderSystem->Vertex2f(0.65f * m_radius, 0.35f * m_radius);
		g_renderSystem->Vertex2f(-0.65f * m_radius, 0.35f * m_radius);

		g_renderSystem->Vertex2f(-0.65f * m_radius, 0.35f * m_radius);
		g_renderSystem->Vertex2f(-0.65f * m_radius, -0.35f * m_radius);

		g_renderSystem->Vertex2f(-0.65f * m_radius, -0.35f * m_radius);
		g_renderSystem->Vertex2f(0.65f * m_radius, -0.35f * m_radius);

		g_renderSystem->Vertex2f(0.65f * m_radius, -0.35f * m_radius);
		g_renderSystem->Vertex2f(0.65f * m_radius, 0.35f * m_radius);

		g_renderSystem->Color3f(.4f, .4f, .7f);

		//The Tip of the Rocket
		g_renderSystem->Vertex2f(0.65f * m_radius, -0.35f * m_radius);
		g_renderSystem->Vertex2f(1.f * m_radius, 0 * m_radius);

		g_renderSystem->Vertex2f(1.f * m_radius, 0 * m_radius);
		g_renderSystem->Vertex2f(0.65f * m_radius, 0.35f * m_radius);

		g_renderSystem->Color3f(1.f, 0.f, 0.f);
	}
	g_renderSystem->GEnd();
	g_renderSystem->BeginLines();
	{
		//The Flame 
		g_renderSystem->Vertex2f(-0.65f*m_radius, -0.3f*m_radius);
		g_renderSystem->Vertex2f(GetRandomFloatInRange((float).70, (float)1.05)*-m_radius, 0 * m_radius);

		g_renderSystem->Vertex2f((float)GetRandomFloatInRange((float).70, (float)1.05)*-m_radius, 0 * m_radius);
		g_renderSystem->Vertex2f(-0.65f*m_radius, 0.3f*m_radius);
	}
	g_renderSystem->GEnd();

	g_renderSystem->PopMatrix();
}
