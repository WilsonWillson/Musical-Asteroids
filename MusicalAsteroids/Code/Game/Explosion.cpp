#include "Game/Explosion.hpp"
#include "Engine/Audio/Audio.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/GameCommon.hpp"

Explosion::Explosion(Vector2 position, float radius, float lifetime)
	:m_position(position),
	m_lifetime(lifetime),
	m_radius(radius)
{

}

Explosion::~Explosion()
{

}

void Explosion::Update(float deltaseconds)
{
	m_lifetime -= deltaseconds;
}

void Explosion::Render()
{	
	const int NUM_VERTEXES = 64;
	const float DEGREES_PER_VERTEX = 360.f / (float)NUM_VERTEXES;
	g_renderSystem->PushMatrix();
	g_renderSystem->Scalef(1.f, 1.f, 1.f);
	g_renderSystem->Color3f(.5f, .5f, 0.f);
	g_renderSystem->BeginPolygon();


	for (int vertexNum = 0; vertexNum < NUM_VERTEXES; ++vertexNum)
	{
		float degrees = DEGREES_PER_VERTEX * (float)vertexNum;
		float x = GetRandomIntInRange(60, 80) * CosDegrees(degrees);
		float y = GetRandomIntInRange(50, 90) * SinDegrees(degrees);

		Vector2 position = Vector2(m_position.x + g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame * x, m_position.y + g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame * y);

		g_renderSystem->Vertex2f(position.x, position.y);
	}
			
	g_renderSystem->GEnd();
	g_renderSystem->PopMatrix();
}





/*
Render Stars Code
const int NUM_VERTEXES = 48;
const float DEGREES_PER_VERTEX = 360.f / (float)NUM_VERTEXES;
g_renderSystem->PushMatrix();
g_renderSystem->Scalef(1.f, 1.f, 1.f);
g_renderSystem->Color3f(1.f, 1.f, 1.f);
g_renderSystem->BeginPolygon();


for (int vertexNum = 0; vertexNum < NUM_VERTEXES; ++vertexNum)
{
float degrees = DEGREES_PER_VERTEX * (float)vertexNum;
float x = GetRandomIntInRange(22, 30) * CosDegrees(degrees);
float y = GetRandomIntInRange(23, 30) * SinDegrees(degrees);

Vector2 position = Vector2(m_position.x + g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame * x, m_position.y + g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame * y);

g_renderSystem->Vertex2f(position.x, position.y);
}

g_renderSystem->GEnd();
g_renderSystem->PopMatrix();

*/