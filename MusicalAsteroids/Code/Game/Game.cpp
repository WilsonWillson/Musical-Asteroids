//Based on code given by Squirrel Eiserloh
#include "Game/Game.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Audio/Audio.hpp"



//---------------------------------Variables--------------------------------------------------------------
Game* g_theGame = nullptr;


//----------------------------------------Creation/Deletion-------------------------------------------------------
Game::Game()
{
	m_currentPlayedSong = nullptr;
	m_ThePlayer = nullptr;
	m_WaveController = nullptr;
	m_Score = 0;

	XboxControl* newControl = new XboxControl();
	m_controller = newControl;
	m_cooldown = 0;

	m_vibrateLeft = m_vibrateRight = m_screenshake = 0;
	m_AsteroidsMissed = m_Streak = 0;
	m_Multiplier = 1;
}

Game::~Game()
{
}

//-----------------------------Start Game----------------------------------------------
void Game::StartNewGame()
{
	PickASong();
	ResetVariables();
}

void Game::PickASong()
{
	//Demo
	//SoundID playerShoot = g_AudioSystem->CreateOrGetSound("Data/Audio/TheWayOfSushido.mp3");
	SoundID playerShoot = g_AudioSystem->CreateOrGetSound("Data/Audio/drumSolo.mp3");



	//Medium
	//SoundID playerShoot = g_AudioSystem->CreateOrGetSound("Data/Audio/seperateways.mp3");

	//Hard
	//SoundID playerShoot = g_AudioSystem->CreateOrGetSound("Data/Audio/TakeOnMe2.mp3");
	//SoundID playerShoot = g_AudioSystem->CreateOrGetSound("Data/Audio/WelcometotheClubNow.mp3");
	//SoundID playerShoot = g_AudioSystem->CreateOrGetSound("Data/Audio/ReadyToLove.mp3");

	//Expert
	//SoundID playerShoot = g_AudioSystem->CreateOrGetSound("Data/Audio/Demonheart.mp3");
	//SoundID playerShoot = g_AudioSystem->CreateOrGetSound("Data/Audio/TheLastStand.mp3");

	m_currentPlayedSong = new AudioChannelHandle();
	*m_currentPlayedSong = g_AudioSystem->PlaySound(playerShoot);
}

void Game::ResetVariables()
{
	m_Score = 0;

	if (m_ThePlayer != nullptr)
		delete m_ThePlayer;

	Entity playerEntity(Vector2(450, 800), Vector2(0, 0), 0.0f, 0.0f, 13.f, 1.0f, 3);
	m_ThePlayer = new Player(playerEntity);

	for (unsigned int cleanStep = 0; cleanStep < m_Enemies.size(); cleanStep++)
	{
		delete m_Enemies[cleanStep];
	}
	m_Enemies.clear();

	for (unsigned int cleanStep = 0; cleanStep < m_Bullets.size(); cleanStep++)
	{
		delete m_Bullets[cleanStep];
	}
	m_Bullets.clear();

	m_StarPositions.clear();
	int amountOfStars = GetRandomIntInRange(15, 30);
	for (int addStars = 0; addStars < amountOfStars; addStars++)
	{
		Vector2 position = Vector2((float)GetRandomIntInRange(0, 1600), (float)GetRandomIntInRange(0, 900));
		m_StarPositions.push_back(position);
	}


	if (m_WaveController != nullptr)
		delete m_WaveController;


	m_WaveController = new WaveController();
}



//--------------------------------------Update---------------------------------------------------------
void Game::Update(float deltaSeconds)
{
	if(m_currentPlayedSong != nullptr)
		g_AudioSystem->FrequencyAnalysis(m_currentPlayedSong, deltaSeconds);

	m_controller->UpdateController(0);

	if (m_controller->m_isButtonDown[START_BUTTON] == true)
		StartNewGame();



	UpdateGameElements(deltaSeconds);

	if(g_AudioSystem->m_IsPlayingSong == true)
		UpdateGameElementsDependingOnSong(deltaSeconds);
}

void Game::UpdateGameElements(float deltaSeconds)
{
	if (m_ThePlayer != nullptr)
		m_ThePlayer->Update(deltaSeconds, m_controller);

	UpdateEnemies(deltaSeconds);
	UpdateBullets(deltaSeconds);
	UpdateExplosions(deltaSeconds);
}

//---------------------------Update Enemies----------------------------------------------------

void Game::UpdateEnemies(float deltaSeconds)
{
	for (unsigned int cleanStep = 0; cleanStep < m_Enemies.size(); cleanStep++)
	{
		if (m_Enemies[cleanStep] != nullptr)
		{
			if (DidCollsionHappenWithPlayerAndEnemy() == false)
			{
				m_Enemies[cleanStep]->Update(deltaSeconds);

				if (m_Enemies[cleanStep]->m_deathTime <= 0 && m_Enemies[cleanStep]->m_wrappingHasHappened == true)
				{
					m_Enemies[cleanStep] = nullptr;
					m_Multiplier = 1;
					m_AsteroidsMissed++;
					m_Streak = 0;
				}
				else if (m_Enemies[cleanStep]->m_deathTime > 0 && m_Enemies[cleanStep]->m_wrappingHasHappened == true)
				{
					m_Enemies[cleanStep]->m_wrappingHasHappened = false;
				}

			}
		}
	}
}

bool Game::DidCollsionHappenWithPlayerAndEnemy()
{
	for (unsigned int enemyStep = 0; enemyStep < m_Enemies.size(); enemyStep++)
	{
		if (m_Enemies[enemyStep] != nullptr)
		{
			float distFromAsteroid = CalcDistance(m_ThePlayer->m_position, m_Enemies[enemyStep]->m_position);
			if (distFromAsteroid < (m_Enemies[enemyStep]->m_PhysicsRadius + m_ThePlayer->m_PhysicsRadius))
			{
				m_Enemies[enemyStep] = nullptr;
				m_ThePlayer->m_health -= 1;

				m_screenshake += 15;
				m_vibrateLeft = 55000;
				m_vibrateRight = 95000;
				return true;
			}
		}
	}
	return false;
}


//---------------------Update Bullets-------------------------------------------------------
void Game::UpdateBullets(float deltaSeconds)
{
	m_cooldown += deltaSeconds;

	if (g_inputSystem->WasKeyJustPressed(KEY_SPACE) == true && m_ThePlayer != nullptr || m_controller->m_isButtonDown[RIGHT_STICK] == true && m_ThePlayer != nullptr && m_cooldown > .25)
	{
		CreateNewBullet();
		m_cooldown = 0;
	}


	
	for (unsigned int cleanStep = 0; cleanStep < m_Bullets.size(); cleanStep++)
	{
		if (m_Bullets[cleanStep] != nullptr)
		{
			if(DidBulletHitAnyAsteroids(m_Bullets[cleanStep]))
				m_Bullets[cleanStep] = nullptr;
			else 
			{
				if (m_Bullets[cleanStep]->m_deathTime > 0)
					m_Bullets[cleanStep]->Update(deltaSeconds);
				else
					m_Bullets[cleanStep] = nullptr;
			}
		}
	}
}

bool Game::DidBulletHitAnyAsteroids(Bullet* bullet)
{
	for (unsigned int asteroidStep = 0; asteroidStep < m_Enemies.size(); asteroidStep++)
	{
		if (m_Enemies[asteroidStep] != nullptr)
		{
			float distFromAsteroid = CalcDistance(bullet->m_position, m_Enemies[asteroidStep]->m_position);
			if (distFromAsteroid < (m_Enemies[asteroidStep]->m_PhysicsRadius + bullet->m_PhysicsRadius))
			{
				m_Score += 100 * m_Multiplier;
				if(m_Multiplier <= 4)
					m_Multiplier += .2f;	
				m_Streak += 1;


				Explosion* newExplosion = new Explosion(m_Enemies[asteroidStep]->m_position, m_Enemies[asteroidStep]->m_radius, EXPLOSION_LIFETIME);
				m_Explosions.push_back(newExplosion);
				m_Enemies[asteroidStep] = nullptr;
				m_screenshake = 3;
				m_vibrateLeft = 40000;
				m_vibrateRight = 60000;
				return true;
			}
		}
	}
	return false;
}


void Game::CreateNewBullet()
{
	Entity newEntity(m_ThePlayer->m_position, Vector2(BULLET_SPEED * CosDegrees(m_controller->m_rightDegrees), BULLET_SPEED * SinDegrees(m_controller->m_rightDegrees)), 0, m_controller->m_rightDegrees, 15, 13, 1);
	Bullet* newBullet = new Bullet(newEntity);
	m_Bullets.push_back(newBullet);
}


//-----------------------------------Update Explosions----------------------------------------------

void Game::UpdateExplosions(float deltaSeconds)
{
	for (unsigned int cleanStep = 0; cleanStep < m_Explosions.size(); cleanStep++)
	{
		if (m_Explosions[cleanStep] != nullptr)
		{
			m_Explosions[cleanStep]->Update(deltaSeconds);
			if (m_Explosions[cleanStep]->m_lifetime < 0)
				m_Explosions[cleanStep] = nullptr;
		}
	}
}


//---------------------------Update Game Based on Audio----------------------------------------------------------
void Game::UpdateGameElementsDependingOnSong(float deltaSeconds)
{
	if (m_WaveController != nullptr)
		m_WaveController->Update(deltaSeconds);

	SpawnAsteroid();

	if (m_vibrateLeft < 6300)
	{
		m_vibrateLeft = g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame * 63000;
		m_vibrateRight = g_AudioSystem->m_averageMediumAllVolumeOfWholeSongLastFrame * 63000;
	}
}


//---------------------------------------------User Input-----------------------------------------------------------------------


void Game::SpawnAsteroidForTesting()
{
	Entity playerEntity(Vector2(450, 800), Vector2(10, 10), 0.0f, 0.0f, 100.f, 60.0f, 100);
	Enemy* newEnemy = new Enemy(playerEntity);

	m_Enemies.push_back(newEnemy);
}

void Game::SpawnAsteroid()
{
	Rgba asteroidColor;
	if (m_WaveController->m_spawnedBasedOnBassThisFrame)
		asteroidColor = Rgba(255, 0, 0, 255);
	if (m_WaveController->m_spawnedBasedOnMidThisFrame)
		asteroidColor = Rgba(0, 255, 0, 255);

	for (unsigned int i = 0; i < m_WaveController->m_PositionsToSpawn.size(); i++)
	{
		Enemy* newEnemy = new Enemy( m_WaveController->m_PositionsToSpawn[i], asteroidColor);

		m_Enemies.push_back(newEnemy);
	}
	m_WaveController->m_PositionsToSpawn.clear();
}


//-----------------------------------Rendering------------------------------------------------------------
void Game::Render()
{
	BasicRenderSetUp();
	RenderGameElements();
	RenderUI();
}

void Game::BasicRenderSetUp()
{
	g_renderSystem->LoadIdentity();
	RenderScreenShake();
	g_renderSystem->ClearColor(0.0f, 0.f, 0.f, 1.f);
	g_renderSystem->ClearColorBufferBit();
}

void Game::RenderScreenShake()
{
	float m_ScreenShakeX = GetRandomFloatInRange(-m_screenshake, m_screenshake);
	float m_ScreenShakeY = GetRandomFloatInRange(-m_screenshake, m_screenshake);
	m_screenshake = m_screenshake * .98f;
	g_renderSystem->Ortho(0.f + m_ScreenShakeX, 1600.f + m_ScreenShakeX, 0.f + m_ScreenShakeY, 900.f + m_ScreenShakeY, 0.f, 1.f);


	m_controller->UpdateVibrationValue(m_vibrateLeft, m_vibrateRight);
	m_vibrateLeft = m_vibrateLeft * .95f;
	m_vibrateRight = m_vibrateRight * .95f;

	
}



void Game::RenderGameElements()
{
	RenderStars();

	if (m_ThePlayer != nullptr)
		m_ThePlayer->Render(m_controller);

	for (unsigned int cleanStep = 0; cleanStep < m_Enemies.size(); cleanStep++)
	{
		if (m_Enemies[cleanStep] != nullptr)
		{
			m_Enemies[cleanStep]->Render();
			//g_renderSystem->DrawLine2D(m_Enemies[cleanStep]->m_position, m_ThePlayer->m_position, Rgba(255, 255, 255, 255), Rgba(255, 255, 255, 255));
		}
	}


	for (unsigned int cleanStep = 0; cleanStep < m_Bullets.size(); cleanStep++)
	{
		if(m_Bullets[cleanStep] != nullptr)
			m_Bullets[cleanStep]->Render();
	}

	for (unsigned int cleanStep = 0; cleanStep < m_Explosions.size(); cleanStep++)
	{
		if (m_Explosions[cleanStep] != nullptr)
			m_Explosions[cleanStep]->Render();
	}

}


void Game::RenderStars()
{

	for (unsigned int i = 0; i < m_StarPositions.size(); i++)
	{
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
			float y = GetRandomIntInRange(22, 30) * SinDegrees(degrees);

			Vector2 position = Vector2(m_StarPositions[i].x + g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame * x, m_StarPositions[i].y + g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame * y);

			g_renderSystem->Vertex2f(position.x, position.y);
		}

		g_renderSystem->GEnd();
		g_renderSystem->PopMatrix();
	}
}



void Game::RenderUI()
{
	g_renderSystem->DrawCenteredText2D(Vector2(50, 850),"Score:", 25, 50, Rgba(255, 255, 255, 255), g_textSprites);
	g_renderSystem->DrawText2D(Vector2(75, 835), std::to_string(m_Score), 25, 100, Rgba(255, 255, 255, 255), g_textSprites);

	g_renderSystem->DrawCenteredText2D(Vector2(75, 800), "Multiplier:", 25,100, Rgba(255, 255, 255, 255), g_textSprites);
	g_renderSystem->DrawText2D(Vector2(130, 785), std::to_string(m_Multiplier), 25, 100, Rgba(255, 255, 255, 255), g_textSprites);

	g_renderSystem->DrawCenteredText2D(Vector2(225, 848), "Streak:", 25, 65, Rgba(255, 255, 255, 255), g_textSprites);
	g_renderSystem->DrawText2D(Vector2(260, 835), std::to_string(m_Streak), 25, 25, Rgba(255, 255, 255, 255), g_textSprites);

	if (m_ThePlayer == nullptr)
	{
		g_renderSystem->DrawText2D(Vector2(400.0f, 550.0f), "MUSICAL Asteroids: Must Be Played with an Xbox 360 Controller", 30.0f, 800, Rgba(255, 255, 255, 255), g_textSprites);
		g_renderSystem->DrawText2D(Vector2(650.0f, 450.0f), "Press F1 to Begin", 30.0f, 300, Rgba(255, 255, 255, 255), g_textSprites);
	}

	if(m_ThePlayer != nullptr)
		m_ThePlayer->RenderUIShips();
}





void Game::RenderDebug()
{
	g_AudioSystem->RenderGraphicsEqualizer(g_renderSystem, g_textSprites);
	m_WaveController->RenderDebug();
}
