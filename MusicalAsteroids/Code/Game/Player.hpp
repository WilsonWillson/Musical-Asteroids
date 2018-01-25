#pragma once
#include "Game/Entity.hpp"
#include "Engine/Input/XboxControl.hpp"


const float SHIP_TURN_DEG_PER_SEC = 180;
const float SHIP_ACCELERATION_STRENGTH = 65;


class Player : public Entity
{
public:
	Player();
	Player(Entity basicSteps);
	~Player();


	void Update(float deltaSeconds, XboxControl* controller);

	void Render(XboxControl* controller);
	void RenderUIShips();

private:
	void ShipAccelerating(float deltaSeconds, float m_thrustMagnitude);
	void ShipTurning(float deltaSeconds, XboxControl* controller);


public:
	Vector2			m_acceleration;
	bool			m_isDead;
	float			m_thrustMagnitude;
	bool			m_RenderVarianceNegative;
};
