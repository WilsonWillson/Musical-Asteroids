#pragma once
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/DX11/DX11Texture2D.hpp"
#include <vector>

class Particle
{
public:
	Particle();
	Particle(Vector4 position, float lifetime);
	~Particle();

public:
	Vector4 m_position;
	float m_lifetime;
};

class SimpleRenderer;
class Renderer;


class ParticleEmitter
{
public:
	ParticleEmitter();
	ParticleEmitter(std::string m_name, Vector4 m_position, Vector4 m_orientation, Vector4 m_velocity, float m_TimeToLive, float m_Scale, bool isLooping);
	~ParticleEmitter();

	void Update(float deltaseconds);

	void RenderParticles(SimpleRenderer* renderer);
	void RenderParticles(Renderer* renderer);


	// stops emission on this effect but allows
	// existing particls to continue to play out.
	void Stop();

	// stops the effect, but allows the existing particles to play out.
	// The system will destroy itself once it is finished (see is_finished)
	void DestroyEffectOnFinish();

	// stops a particle effect, immeidately destroying 
	// all of its existing particles.
	void DestroyImmediate();

	// resumes playing of an effect
	void Play();

	// Destroys existing particles, puts effect
	// back in it's initial state [will run initial burst and prewarm]
	void Reset();

	// Should spawn particles and simulat the system up to the max
	// lifetime of the longest living particle
	// [optionally pass it a set time]
	void Prewarm(float prewarmAmount);

	// no systems will spawn and all particles have met a natural end.
	// (a stopped particle system is considered finished once its particles have played out)
	bool IsFinished();

	void DeleteCurrentParticles();


public:
	std::string m_name;
	std::string m_Shape;
	std::string m_blendMode;
	Vector4 m_position;
	Vector4 m_orientation;
	Rgba m_tint;

	float m_TimeToLive;
	float m_Scale;
	float m_SpawnRate;
	float m_TimeSinceLastSpawn;
	float m_SpawnBurst;
//	DX11Texture2D texture;
	Vector4 m_velocity;
	float initalLiveTime;



	bool m_isLooping;
	bool m_pauseSpawning;
	bool m_finishedSpawning;
	bool m_debugRenderPosition;

	std::vector<Particle*> m_particles;
};