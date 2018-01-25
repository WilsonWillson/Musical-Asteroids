#pragma once
#include "Game/Entity.hpp"

class Bullet : public Entity
{
public:
	Bullet();
	Bullet(Entity bulletStats);
	~Bullet();


	void Update(float deltaSeconds);
	void Render();

public:
	int			m_deathTime;
};
