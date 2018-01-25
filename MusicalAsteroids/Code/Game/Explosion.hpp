#include "Engine/Math/Vector2.hpp"


class Explosion
{
public:
	Explosion(Vector2 position, float radius, float lifetime);
	~Explosion();


	void Update(float deltaseconds);
	void Render();


public:
	Vector2 m_position;
	float m_radius;
	float m_lifetime;


};