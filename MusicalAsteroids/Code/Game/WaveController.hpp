#pragma once
#include "Game/Entity.hpp"
#include "Engine/Math/Vector2.hpp"
#include <vector>

const int MINIMUM_SPEED = 100;

const float BASS_VOLUME_NEEDED_TO_SPAWN = .21f;
const float BASS_VOLUME_NEEDED_TO_IGNORE_SPAWN_TIMER = .5f;

const float MID_VOLUME_NEEDED_TO_SPAWN = .137f;
const float MID_VOLUME_NEEDED_TO_IGNORE_SPAWN_TIMER = .165f;
const float MID_VOLUME_SPIKE_NEEDED_TO_SPAWN = .19f;
const float MID_VOLUME_BIG_SPIKE_NEEDED_TO_SPAWN = .215f;


class WaveController
{
public:
	WaveController();
	~WaveController();

	void Update(float deltaseconds);

	void HowManyEnemiesToSpawn();
	void CreateNewAsteroid(int spawnIndex);



	void RenderDebug();

private:
	void CreateAsteroidBasedOffMedium();
	void CreateAsteroidBasedOffBass();
	
	
	void SpawnOneAsteroid();
	void SpawnTwoAsteroids();
	void SpawnThreeAsteroids();
	void SpawnFourAsteroids();


public:
	float    m_LastTimeSinceSpawn;
	float    m_LastTimeSinceLastBassSpawn;
	float    m_LastTimeSinceLastMidSpawn;
	int		 m_HowManyToSpawn;
	std::vector<Entity> m_PositionsToSpawn;
	std::vector<float>	m_LastTimeCertainDirectionsSpawned;



	bool m_spawnedAsteroidThisFrame;
	bool m_spawnedBasedOnBassThisFrame;
	bool m_spawnedBasedOnMidThisFrame;
};