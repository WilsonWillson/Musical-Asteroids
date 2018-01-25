#include "Game/WaveController.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Audio/Audio.hpp"

//------------------------Constructor/Deconstructor-----------------------------------------
WaveController::WaveController()
{
	for (int i = 0; i < 8; i++)
	{
		m_LastTimeCertainDirectionsSpawned.push_back(0);
	}

	m_LastTimeSinceSpawn = 0;
}

WaveController::~WaveController()
{

}

//-----------------------------------Update-----------------------------------------------------
void WaveController::Update(float deltaseconds)
{
	m_LastTimeSinceSpawn -= deltaseconds;
	m_LastTimeSinceLastBassSpawn -= deltaseconds;
	m_LastTimeSinceLastMidSpawn -= deltaseconds;


	for (unsigned int i = 0; i < m_LastTimeCertainDirectionsSpawned.size(); i++)
	{
		m_LastTimeCertainDirectionsSpawned[i] += deltaseconds;
	}

	m_spawnedAsteroidThisFrame = false;
	m_spawnedBasedOnBassThisFrame = false;
	m_spawnedBasedOnMidThisFrame = false;

	HowManyEnemiesToSpawn();

}

//------------------------Wave Control Logic--------------------------------------------------------


void WaveController::HowManyEnemiesToSpawn()
{
	CreateAsteroidBasedOffMedium();
	CreateAsteroidBasedOffBass();
}


void WaveController::CreateAsteroidBasedOffMedium()
{

	float howManyToSpawn = 0;
	float tempTimer = 0.f;
	for (int i = 0; i < MEDIUMALL_SIZE; i++)
	{
		if (m_LastTimeSinceLastMidSpawn < 0 && g_AudioSystem->m_averageMediumAllVolumeLastFrame[i] > MID_VOLUME_NEEDED_TO_SPAWN 
			|| m_LastTimeSinceLastMidSpawn < 1.5 && g_AudioSystem->m_averageMediumAllVolumeLastFrame[i] > MID_VOLUME_NEEDED_TO_IGNORE_SPAWN_TIMER)
		{
			howManyToSpawn++;
			m_spawnedBasedOnMidThisFrame = true;
			
			tempTimer += 1;
		}

		if (m_LastTimeSinceLastMidSpawn < 1.5 && g_AudioSystem->m_averageMediumAllVolumeLastFrame[i] > MID_VOLUME_SPIKE_NEEDED_TO_SPAWN)
		{
			howManyToSpawn = 4;
			m_spawnedBasedOnMidThisFrame = true;

			tempTimer = 4;
		}

		if (m_LastTimeSinceLastMidSpawn < 1 && g_AudioSystem->m_averageMediumAllVolumeLastFrame[i] > MID_VOLUME_BIG_SPIKE_NEEDED_TO_SPAWN)
		{
			howManyToSpawn = 4;
			m_spawnedBasedOnMidThisFrame = true;

			tempTimer = 4;
		}
	}




	if (howManyToSpawn > 4)
		howManyToSpawn = 4;


	if (howManyToSpawn > 0)
	{
		if (m_LastTimeSinceLastMidSpawn < 0)
			m_LastTimeSinceLastMidSpawn = 0;

		m_LastTimeSinceLastMidSpawn = tempTimer;
		m_LastTimeSinceLastBassSpawn = tempTimer * .60f;


		CreateNewAsteroid((int)howManyToSpawn);
	}

}

void WaveController::CreateAsteroidBasedOffBass()
{
	if (m_LastTimeSinceLastBassSpawn < 0 &&  g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame > BASS_VOLUME_NEEDED_TO_SPAWN)
	{
		CreateNewAsteroid(1);
		m_spawnedBasedOnBassThisFrame = true;
		m_LastTimeSinceLastBassSpawn = 1.5f;
	}


	if (m_LastTimeSinceLastBassSpawn < 1.75 && g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame > BASS_VOLUME_NEEDED_TO_IGNORE_SPAWN_TIMER)
	{
		CreateNewAsteroid(2);
		m_spawnedBasedOnBassThisFrame = true;
		m_LastTimeSinceLastBassSpawn += 1.5f;
	}
}




//-----------------------------Spawn Asteroid---------------------------------------------------------------------
void WaveController::CreateNewAsteroid(int spawnIndex)
{
	if(spawnIndex == 1)
		SpawnOneAsteroid();
	if (spawnIndex == 2)
		SpawnTwoAsteroids();
	if (spawnIndex == 3)
		SpawnThreeAsteroids();
	if (spawnIndex == 4)
		SpawnFourAsteroids();

	m_spawnedAsteroidThisFrame = true;
}

void WaveController::SpawnOneAsteroid()
{
	float xPosition = 0;
	float yPosition = 0;
	Vector2 velocity;

	int spawnIndex = rand() % 12;

	if (spawnIndex == 0)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(450, 850);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED;
		velocity.y = 0;
	}
	if (spawnIndex == 1)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(0, 450);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED;
		velocity.y = 0;
	}
	if (spawnIndex == 2)
	{
		xPosition = GetRandomFloatInRange(0, 800);
		yPosition = GetRandomFloatInRange(-100, 0);

		float randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = 0;
		velocity.y = g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED;
	}
	if (spawnIndex == 3)
	{
		xPosition = GetRandomFloatInRange(800, 1550);
		yPosition = GetRandomFloatInRange(-100, 0);

		float randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = 0;
		velocity.y = g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED;
	}
	if (spawnIndex == 4)
	{
		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(0, 450);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;
	}
	if (spawnIndex == 5)
	{
		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(450, 850);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;
	}
	if (spawnIndex == 6)
	{
		xPosition = GetRandomFloatInRange(800, 1550);
		yPosition = GetRandomFloatInRange(900, 1000);


		float randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = 0;
		velocity.y = -(g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED);
	}
	if (spawnIndex == 7)
	{
		xPosition = GetRandomFloatInRange(0, 800);
		yPosition = GetRandomFloatInRange(900, 1000);


		float randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = 0;
		velocity.y = -( g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED);
	}
	if (spawnIndex == 8)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(900, 1000);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);
		float randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED;
		velocity.y = -(g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED);
	}
	if (spawnIndex == 9)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(-100, 0);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);
		float randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED;
		velocity.y = g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED;
	}
	if (spawnIndex == 10)
	{
		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(900, 1000);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);
		float randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = -(g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED);
	}
	if (spawnIndex == 11)
	{
		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(-100, 0);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);
		float randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED;
	}

	m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
}

void WaveController::SpawnTwoAsteroids()
{
	float xPosition = 0;
	float yPosition = 0;
	Vector2 velocity;

	int spawnIndex = rand() % 16;

	spawnIndex = 14;
	if (spawnIndex == 0)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(0, 450);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED;
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		yPosition += 400;
		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 1)
	{
		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(0, 450);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		yPosition += 400;
		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 2)
	{
		xPosition = GetRandomFloatInRange(0, 800);
		yPosition = GetRandomFloatInRange(-100, 0);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = 0;
		velocity.y = g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition += 600;
		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 3)
	{
		xPosition = GetRandomFloatInRange(0, 800);
		yPosition = GetRandomFloatInRange(900, 1000);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = 0;
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition += 600;
		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 4)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(50, 850);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition += GetRandomFloatInRange(1600, 1700);
		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 5)
	{
		xPosition = GetRandomFloatInRange(0, 1550);
		yPosition = GetRandomFloatInRange(-100, 0);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);
		float randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = 0;
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		yPosition += GetRandomFloatInRange(900, 1000);
		velocity.y = -(g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED);
		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 6)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(-100, 0);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(900, 1000);

		velocity.x = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 7)
	{
		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(-100, 0);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(900, 1000);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 8)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(-100, 0);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 9)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(900, 1000);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(900, 1000);

		velocity.x = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 10)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(-100, 0);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(-100, 0);

		velocity.x = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 11)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(900, 1000);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		
		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(900, 1000);
		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 12)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(0, 850);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(0, 1550);
		yPosition = GetRandomFloatInRange(-100, 0);

		velocity.x = 0;
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 13)
	{
		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(0, 850);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(0, 1550);
		yPosition = GetRandomFloatInRange(-100, 0);

		velocity.x = 0;
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 14)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(0, 850);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(0, 1550);
		yPosition = GetRandomFloatInRange(1600, 1700);

		velocity.x = 0;
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 15)
	{
		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(0, 850);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(0, 1550);
		yPosition = GetRandomFloatInRange(1600, 1700);

		velocity.x = 0;
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}
}

void WaveController::SpawnThreeAsteroids()
{
	float xPosition = 0;
	float yPosition = 0;
	Vector2 velocity;

	int spawnIndex = rand() % 12;

	spawnIndex = 11;
	if (spawnIndex == 0)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(0, 250);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED;
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		yPosition += 300;
		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		yPosition += 300;
		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 1)
	{
		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(0, 250);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = -( g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		yPosition += 300;
		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		yPosition += 300;
		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 2)
	{
		xPosition = GetRandomFloatInRange(0, 500);
		yPosition = GetRandomFloatInRange(-100, 0);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = 0;
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition += 500;
		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition += 500;
		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 3)
	{
		xPosition = GetRandomFloatInRange(0, 500);
		yPosition = GetRandomFloatInRange(900, 1000);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = 0;
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition += 500;
		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition += 500;
		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 4)
	{
		xPosition = GetRandomFloatInRange(750, 850);
		yPosition = GetRandomFloatInRange(900, 1000);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = 0;
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(900, 1000);

		velocity.x = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(900, 1000);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 5)
	{
		xPosition = GetRandomFloatInRange(750, 850);
		yPosition = GetRandomFloatInRange(-100, 0);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = 0;
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(-100, 0);

		velocity.x = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(-100, 0);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 6)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(450, 550);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(-100, 0);

		velocity.x = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(900, 1000);

		velocity.x = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 7)
	{
		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(450, 550);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(-100, 0);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(900, 1000);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 8)
	{
		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(450, 550);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(450, 550);

		velocity.x = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(750, 850);
		yPosition = GetRandomFloatInRange(-100, 0);

		velocity.x = 0;
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 9)
	{
		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(450, 550);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(450, 550);

		velocity.x = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(750, 850);
		yPosition = GetRandomFloatInRange(1600, 1700);

		velocity.x = 0;
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 10)
	{
		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(450, 550);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(750, 850);
		yPosition = GetRandomFloatInRange(1600, 1700);

		velocity.x = 0;
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(750, 850);
		yPosition = GetRandomFloatInRange(-100, 0);

		velocity.x = 0;
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 11)
	{
		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(450, 550);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);

		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(750, 850);
		yPosition = GetRandomFloatInRange(1600, 1700);

		velocity.x = 0;
		velocity.y = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(750, 850);
		yPosition = GetRandomFloatInRange(-100, 0);

		velocity.x = 0;
		velocity.y = (g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}
}

void WaveController::SpawnFourAsteroids()
{
	float xPosition = 0;
	float yPosition = 0;
	Vector2 velocity;

	int spawnIndex = rand() %6;



	if (spawnIndex == 0)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(900, 1000);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);
		float randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED;
		velocity.y = -(g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(-100, 0);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED;
		velocity.y = g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(900, 1000);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = -(g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(-100, 0);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}
	
	if (spawnIndex == 1)
	{
		xPosition = GetRandomFloatInRange(750, 850);
		yPosition = GetRandomFloatInRange(-45, -55);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);
		float randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = 0;
		velocity.y = g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(750, 850);
		yPosition = GetRandomFloatInRange(945, 955);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = 0;
		velocity.y = -(g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(-45, -55);
		yPosition = GetRandomFloatInRange(440, 460);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED;
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(1645, 1655);
		yPosition = GetRandomFloatInRange(440, 460);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y =0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 2)
	{
		xPosition = GetRandomFloatInRange(50, 400);
		yPosition = GetRandomFloatInRange(900, 1000);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);
		float randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = 0;
		velocity.y = -(g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(400, 800);
		yPosition = GetRandomFloatInRange(900, 1000);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = 0;
		velocity.y = -(g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(800, 1200);
		yPosition = GetRandomFloatInRange(900, 1000);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = 0;
		velocity.y = -(g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(1200, 1550);
		yPosition = GetRandomFloatInRange(900, 1000);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = 0;
		velocity.y = -(g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED);

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 3)
	{
		xPosition = GetRandomFloatInRange(50, 400);
		yPosition = GetRandomFloatInRange(-100, 0);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);
		float randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = 0;
		velocity.y = g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(400, 800);
		yPosition = GetRandomFloatInRange(-100, 0);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = 0;
		velocity.y = g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(800, 1200);
		yPosition = GetRandomFloatInRange(-100, 0);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = 0;
		velocity.y = g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(1200, 1550);
		yPosition = GetRandomFloatInRange(-100, 0);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = 0;
		velocity.y = g_AudioSystem->m_BPM * randomY + MINIMUM_SPEED;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 4)
	{
		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(50, 225);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);
		float randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED;
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(225, 450);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED;
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(450, 675);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED;
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(-100, 0);
		yPosition = GetRandomFloatInRange(675, 850);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED;
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}

	if (spawnIndex == 5)
	{
		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(50, 225);

		float randomX = GetRandomFloatInRange(.5f, 1.0f);
		float randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(225, 450);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(450, 675);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));

		xPosition = GetRandomFloatInRange(1600, 1700);
		yPosition = GetRandomFloatInRange(675, 850);

		randomX = GetRandomFloatInRange(.5f, 1.0f);
		randomY = GetRandomFloatInRange(.5f, 1.0f);
		velocity.x = -(g_AudioSystem->m_BPM * randomX + MINIMUM_SPEED);
		velocity.y = 0;

		m_PositionsToSpawn.push_back(Entity(Vector2(xPosition, yPosition), velocity, 0.0f, 0.0f, 50.f, 50.0f, 100));
	}
}


//---------------------------------------Render---------------------------------------------------
void WaveController::RenderDebug()
{

}
