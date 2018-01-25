#pragma once
#include "Game/Entity.hpp"
#include "Engine/Core/Rgba.hpp"

const int ASTEROID_NUM_SIZE = 17;
const float DEATH_TIME = 8;
const float START_RENDER_TIME = 1;

class Enemy : public Entity
{
public:
	Enemy();
	Enemy(Entity enemyStats);
	Enemy(Entity enemyStats, Rgba asteroidColor);
	~Enemy();


	void Update(float deltaSeconds);
	void Render();

public:
	Vector2 m_vertexes[ASTEROID_NUM_SIZE];
	Rgba m_AsteroidColor;
	int m_size;
	float m_deathTime;
	Vector2 m_StartingLocation;
	float     m_StartLocationRenderRemaining;


private:
	void RenderStartingLocation();
};
