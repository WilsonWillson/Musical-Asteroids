#include "Game/Enemy.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/GameCommon.hpp"

//-----------------------Constructor/Deconstructor---------------------------------
Enemy::Enemy()
	:Entity(),
	m_deathTime(DEATH_TIME),
	m_StartLocationRenderRemaining(START_RENDER_TIME)
{

}

Enemy::Enemy(Entity enemyStats, Rgba asteroidColor)
	: Entity(enemyStats),
	m_size(3),
	m_deathTime(DEATH_TIME),
	m_AsteroidColor(asteroidColor),
	m_StartLocationRenderRemaining(START_RENDER_TIME)
{
	Vector2 asteroidCoordinate;
	const float DEGREES_PER_VERTEX = 360.f / (float)ASTEROID_NUM_SIZE;
	{
		for (int vertexNum = 0; vertexNum < ASTEROID_NUM_SIZE; ++vertexNum)
		{
			float varyingRadius = GetRandomFloatInRange(0.75f, 1.1f);
			float degrees = DEGREES_PER_VERTEX * (float)vertexNum;
			degrees += m_orientationDegrees;
			asteroidCoordinate.x = varyingRadius * CosDegrees(degrees);
			asteroidCoordinate.y = varyingRadius * SinDegrees(degrees);
			m_vertexes[vertexNum] = asteroidCoordinate;
		}
	}

	m_StartingLocation = m_position;
}

Enemy::Enemy(Entity enemyStats)
	: Entity(enemyStats),
	m_size(3),
	m_deathTime(DEATH_TIME),
	m_StartLocationRenderRemaining(START_RENDER_TIME)
{
	Vector2 asteroidCoordinate;
	const float DEGREES_PER_VERTEX = 360.f / (float)ASTEROID_NUM_SIZE;
	{
		for (int vertexNum = 0; vertexNum < ASTEROID_NUM_SIZE; ++vertexNum)
		{
			float varyingRadius = GetRandomFloatInRange(0.75f, 1.1f);
			float degrees = DEGREES_PER_VERTEX * (float)vertexNum;
			degrees += m_orientationDegrees;
			asteroidCoordinate.x = varyingRadius * CosDegrees(degrees);
			asteroidCoordinate.y = varyingRadius * SinDegrees(degrees);
			m_vertexes[vertexNum] = asteroidCoordinate;
		}
	}

	m_StartingLocation = m_position;

	m_AsteroidColor = Rgba(146, 98, 57, 255);
}

Enemy::~Enemy()
{

}

//--------------------------Update-----------------------------------------------------
void Enemy::Update(float deltaSeconds)
{
	m_deathTime -= deltaSeconds;
	m_StartLocationRenderRemaining -= deltaSeconds /2;
	m_orientationDegrees += (m_spinDegreesPerSecond * deltaSeconds);
	m_position.x += (m_velocity.x * deltaSeconds);
	m_position.y += (m_velocity.y * deltaSeconds);
	DetermineWrapping();
}

//-----------------------------Rendering------------------------------------------------
void Enemy::Render()
{
	g_renderSystem->PushMatrix();
	g_renderSystem->Translatef(m_position.x, m_position.y, 0.f);
	g_renderSystem->Rotatef(m_orientationDegrees, 0.f, 0.f, 1.f);
	g_renderSystem->Scalef(m_radius, m_radius, 1.f);
	g_renderSystem->Color3f(m_AsteroidColor.r, m_AsteroidColor.g, m_AsteroidColor.b);
	g_renderSystem->BeginPolygon();
	{
		for (int vertexNum = 0; vertexNum < ASTEROID_NUM_SIZE; ++vertexNum)
		{
			g_renderSystem->Vertex2f(m_vertexes[vertexNum].x, m_vertexes[vertexNum].y);
		}
	}

	g_renderSystem->GEnd();

	//g_renderSystem->drawCircle(1, m_AsteroidColor, Vector2(0, 0));

	g_renderSystem->PopMatrix();

	if(m_StartLocationRenderRemaining > 0)
		RenderStartingLocation();
}

void Enemy::RenderStartingLocation()
{
	Rgba StartParticleColor = Rgba(m_AsteroidColor.r, m_AsteroidColor.g, m_AsteroidColor.b, 255 * m_StartLocationRenderRemaining);
	
	Vector2 endLocation = m_StartingLocation;

	if (endLocation.y > 900)
		endLocation.y -= 200;
	else
		endLocation.y += 200;


	Vector2 endLocation2 = endLocation;
	
	if (endLocation.x > 1600)
	{
		endLocation.x -= 200;
		endLocation2.x -= 200;
		endLocation2.y -= 400;
	}
	else if (endLocation.x < 0)
	{
		endLocation.x += 200;
		endLocation2.x += 200;
		endLocation2.y -= 400;
	}
	else
	{
		endLocation.x += 200;
		endLocation2.x -= 200;
	}


	g_renderSystem->DrawLine2D(m_StartingLocation, endLocation, StartParticleColor, StartParticleColor);
	g_renderSystem->DrawLine2D(m_StartingLocation, endLocation2, StartParticleColor, StartParticleColor);
}
