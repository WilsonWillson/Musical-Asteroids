#pragma once
#include "Engine/Input/XboxControl.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Player.hpp"
#include "Game/Enemy.hpp"
#include "Game/Bullet.hpp"
#include "Game/Explosion.hpp"
#include "Game/WaveController.hpp"



//-----------------------------------------------------------------------------------------------

typedef void* AudioChannelHandle;

const float BULLET_SPEED = 450.f;
const float EXPLOSION_LIFETIME = .25f;


class Game
{
public:
	Game();
	~Game();

	void Update(float deltaSeconds);

	void StartNewGame();

	void Render();
	void RenderDebug();

	void SpawnAsteroidForTesting();

private:
	void PickASong();	
	void ResetVariables();

	void UpdateGameElements(float deltaSeconds);
	
	void UpdateBullets(float deltaSeconds);
	bool DidBulletHitAnyAsteroids(Bullet* bullet);
	void CreateNewBullet();

	void UpdateEnemies(float deltaSeconds);
	bool DidCollsionHappenWithPlayerAndEnemy();


	void UpdateGameElementsDependingOnSong(float deltaSeconds);
	void SpawnAsteroid();

	void BasicRenderSetUp();
	void RenderGameElements();
	void RenderUI();
	void RenderScreenShake();


	void UpdateExplosions(float deltaSeconds);
	void RenderStars();
public:
	AudioChannelHandle*		m_currentPlayedSong;
	XboxControl*			m_controller;

	Player*					m_ThePlayer;
	std::vector<Enemy*>		m_Enemies;
	std::vector<Explosion*>	m_Explosions;
	std::vector<Bullet*>	m_Bullets;
	std::vector<Vector2>	m_StarPositions;
	WaveController*			m_WaveController;



	float					m_Score;
	float					m_cooldown;
	float					m_screenshake;
	float					m_vibrateLeft;
	float					 m_vibrateRight;

	float					m_Multiplier;
	int						m_Streak;
	int						m_AsteroidsMissed;
};


//-----------------------------------------------------------------------------------------------
extern Game* g_theGame;

