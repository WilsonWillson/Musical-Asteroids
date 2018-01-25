#include "Game/Player.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Audio/Audio.hpp"
#include "Engine/Math/MathUtils.hpp"

//--------------------Constructor-------------------------------------------
Player::Player()
	:Entity()
	, m_acceleration(0.f, 0.f)
	, m_isDead(false)
	, m_thrustMagnitude(0.f)
{

}

Player::Player(Entity basicSteps)
	:Entity(basicSteps)
	, m_acceleration(0.f, 0.f)
	, m_isDead(false)
	, m_thrustMagnitude(0.f)
{

}

Player::~Player()
{

}


//---------------------------------Update------------------------------
void Player::Update(float deltaSeconds, XboxControl* controller)
{
	ShipTurning(deltaSeconds, controller);
	m_thrustMagnitude = controller->GetLeftRadius();
	if (g_inputSystem->IsKeyDown(KEY_W) == true || g_inputSystem->IsKeyDown(KEY_UPARROW) == true)
		m_thrustMagnitude = 1.0;
	if (g_inputSystem->IsKeyDown(KEY_W) == true || g_inputSystem->IsKeyDown(KEY_UPARROW) == true || controller->m_isButtonDown[LEFT_STICK] == true) {
		ShipAccelerating(deltaSeconds, m_thrustMagnitude);
	}
	m_position += m_velocity * deltaSeconds;
	DetermineWrapping();

}

void Player::ShipTurning(float deltaSeconds, XboxControl* Controller)
{
	float spin = 0.f;
	if (g_inputSystem->IsKeyDown(KEY_A) == true || g_inputSystem->IsKeyDown(KEY_LEFTARROW) == true)
		spin += SHIP_TURN_DEG_PER_SEC * deltaSeconds;
	if (g_inputSystem->IsKeyDown(KEY_D) == true || g_inputSystem->IsKeyDown(KEY_RIGHTARROW) == true)
		spin -= SHIP_TURN_DEG_PER_SEC * deltaSeconds;
	m_orientationDegrees += spin;
	if (Controller->m_isButtonDown[LEFT_STICK] == true)
		m_orientationDegrees = Controller->m_leftDegrees;
	if (Controller->m_isButtonDown[RIGHT_STICK] == true)
		m_orientationDegrees = Controller->m_rightDegrees;
}


void Player::ShipAccelerating(float deltaSeconds, float shipMagnitude)
{
	float degrees = m_orientationDegrees;
	float length = SHIP_ACCELERATION_STRENGTH * shipMagnitude;
	m_acceleration.SetLengthAndYawDegrees(length, degrees);
	m_velocity += m_acceleration * deltaSeconds;
}





//----------------------------------Render---------------------------------
void Player::Render(XboxControl* controller)
{

	g_renderSystem->PushMatrix();
	g_renderSystem->Translatef(m_position.x, m_position.y, 0.f);
	g_renderSystem->Rotatef(m_orientationDegrees, 0.f, 0.f, 1.f);
	g_renderSystem->Rotatef(90, 0.f, 0.f, 1.f);

	float volumeControlDrawBass = g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame * 10;
	float volumeControlDrawMid = g_AudioSystem->m_averageMediumHighVolumeOfWholeSongLastFrame * 10;

	if (m_RenderVarianceNegative == true)
	{
		volumeControlDrawBass = -volumeControlDrawBass;
		volumeControlDrawMid = -volumeControlDrawMid;
	}
	m_RenderVarianceNegative = !m_RenderVarianceNegative;

	g_renderSystem->DrawLine2D(Vector2(0 + volumeControlDrawMid,-15), Vector2(-10 + volumeControlDrawBass, 15), Rgba(255, 255, 255, 255), Rgba(255, 255, 255, 255));
	g_renderSystem->DrawLine2D(Vector2(-10 + volumeControlDrawMid, 15), Vector2(-7 + volumeControlDrawBass, 10), Rgba(255, 255, 255, 255), Rgba(255, 255, 255, 255));

	g_renderSystem->DrawLine2D(Vector2(-7 + volumeControlDrawMid, 10), Vector2(7 + volumeControlDrawMid, 10), Rgba(255, 255, 255, 255), Rgba(255, 255, 255, 255));

	g_renderSystem->DrawLine2D(Vector2(10 + volumeControlDrawMid,15), Vector2(7 + volumeControlDrawBass,  10), Rgba(255, 255, 255, 255), Rgba(255, 255, 255, 255));
	g_renderSystem->DrawLine2D(Vector2(0 + volumeControlDrawMid, -15), Vector2(10 + volumeControlDrawBass, 15), Rgba(255, 255, 255, 255), Rgba(255, 255, 255, 255));

	//g_renderSystem->drawCircle(m_radius, Rgba(255,0,0,255), Vector2(0,0) );



	if (g_inputSystem->IsKeyDown(KEY_W) == true || g_inputSystem->IsKeyDown(KEY_UPARROW) == true || controller->m_isButtonDown[LEFT_STICK] == true) {
		float flameDistance = fabs(m_thrustMagnitude);
		if (flameDistance < .51f)
			flameDistance = .51f;

		g_renderSystem->DrawLine2D(Vector2(volumeControlDrawMid  + -7 * flameDistance, 10), Vector2(0, abs(volumeControlDrawBass) * 15 + 20 * flameDistance), Rgba(255, 0, 0, 255), Rgba(255, 0, 0, 255));
		g_renderSystem->DrawLine2D(Vector2(volumeControlDrawMid +  7 * flameDistance, 10), Vector2(0, abs(volumeControlDrawBass) * 15 + 20 * flameDistance), Rgba(255, 0, 0, 255), Rgba(255, 0, 0, 255));
	}
	g_renderSystem->PopMatrix();
}

void Player::RenderUIShips()
{
	Vector2 uiPosition(1500, 875);
	float uiPositionCounter = 30.0f;

	for (int playerLives = 0; playerLives < m_health; playerLives++)
	{
		g_renderSystem->PushMatrix();
		g_renderSystem->Translatef(uiPosition.x, uiPosition.y, 0.f);
		g_renderSystem->Rotatef(90.0f, 0.f, 0.f, 1.f);
		g_renderSystem->Rotatef(90, 0.f, 0.f, 1.f);

		g_renderSystem->DrawLine2D(Vector2(0, 0), Vector2(-10, 30), Rgba(255, 255, 255, 255), Rgba(255, 255, 255, 255));
		g_renderSystem->DrawLine2D(Vector2(-10, 30), Vector2(-7, 25), Rgba(255, 255, 255, 255), Rgba(255, 255, 255, 255));

		g_renderSystem->DrawLine2D(Vector2(-7, 25), Vector2(7, 25), Rgba(255, 255, 255, 255), Rgba(255, 255, 255, 255));

		g_renderSystem->DrawLine2D(Vector2(10, 30), Vector2(7, 25), Rgba(255, 255, 255, 255), Rgba(255, 255, 255, 255));
		g_renderSystem->DrawLine2D(Vector2(0, 0), Vector2(10, 30), Rgba(255, 255, 255, 255), Rgba(255, 255, 255, 255));

		g_renderSystem->PopMatrix();

		uiPosition.x += uiPositionCounter;
	}
}
