#include "Engine/ParticleSystem/ParticleEmitter.hpp"
#include "Engine/RHI/SimpleRenderer.hpp"
#include "Engine/Renderer/Renderer.hpp"

//------------------------Particle Code-----------------------------------------------
Particle::Particle()
{
	m_position = Vector4(-1, -1, -1, -1);
	m_lifetime = -999;
}

Particle::Particle(Vector4 position, float lifetime)
	:m_position(position),
	m_lifetime(lifetime)
{

}

Particle::~Particle()
{
	
}



//--------------------------Particle Emitters------------------------------
ParticleEmitter::ParticleEmitter(std::string name, Vector4 position, Vector4 orientation, Vector4 velocity, float timeToLive, float scale, bool isLooping)
	:m_name(name),
	m_position(position),
	m_orientation(orientation),
	m_TimeToLive(timeToLive),
	initalLiveTime(timeToLive),
	m_Scale(scale),
	m_velocity(velocity),
	m_isLooping(isLooping)
{
	m_debugRenderPosition = false;
	m_pauseSpawning = false;
	m_finishedSpawning = false;
	m_TimeSinceLastSpawn = 0;
}

ParticleEmitter::ParticleEmitter()
{
	m_debugRenderPosition = false;
	m_pauseSpawning = false;
	m_finishedSpawning = false;
}

ParticleEmitter::~ParticleEmitter()
{
	DeleteCurrentParticles();
}


//--------------------------------------Update------------------------------------------------------
void ParticleEmitter::Update(float deltaseconds)
{
	//Do Lifetime Management
	m_TimeToLive -= deltaseconds;
	m_TimeSinceLastSpawn -= deltaseconds;
	if (m_TimeToLive < 0 && m_isLooping == false || IsFinished() == true)
		DestroyEffectOnFinish();

	//AddNewParticles
	if (m_TimeSinceLastSpawn <= 0)
	{
		for (int i = 0; i < m_SpawnBurst; i++)
		{
			Particle* newParticle = new Particle();
			newParticle->m_lifetime = initalLiveTime;
			newParticle->m_position = m_position;
			m_particles.push_back(newParticle);
		}
		m_TimeSinceLastSpawn = m_SpawnRate;
	}


	//Update the Particles Themselfs. Apply Physics and stuff	
	for (unsigned int i = 0; i < m_particles.size(); i++)
	{
		if (m_particles[i] != nullptr)
		{
			m_particles[i]->m_lifetime -= deltaseconds;
			Vector4 tempVelocity = Vector4(m_velocity.x * deltaseconds, m_velocity.y * deltaseconds, m_velocity.z *deltaseconds, m_velocity.w * deltaseconds);
			m_particles[i]->m_position = m_particles[i]->m_position + tempVelocity;

			if (m_particles[i]->m_lifetime < 0)
			{
				delete m_particles[i];
				m_particles[i] = nullptr;
			}
		}
	}
}


//---------------------------------Render---------------------------------------------------------------------------------
void ParticleEmitter::RenderParticles(SimpleRenderer* renderer)
{
	if(m_blendMode == "alpha")
		renderer->enable_blend(BLEND_SRC_ALPHA, BLEND_INV_SRC_ALPHA);
	if (m_blendMode == "one")
		renderer->enable_blend(BLEND_ONE, BLEND_ONE);
	if (m_blendMode == "zero")
		renderer->enable_blend(BLEND_ZERO, BLEND_ZERO);
	if (m_blendMode == "color")
		renderer->enable_blend(BLEND_SRC_COLOR, BLEND_DEST_COLOR);


	renderer->enable_blend(BLEND_SRC_ALPHA, BLEND_INV_SRC_ALPHA);
	for (unsigned int i = 0; i < m_particles.size(); i++)
	{
		if (m_particles[i] != nullptr)
		{
			Rgba currentTint = m_tint;
			currentTint.a = (unsigned char) RangeMapFloat((float)m_particles[i]->m_lifetime, 0.f, (float)initalLiveTime, 0.f, 255.f);

			if (m_particles[i] != nullptr && m_Shape == "Plane")
				renderer->drawPlane(Vector3(m_particles[i]->m_position.x, m_particles[i]->m_position.y, m_particles[i]->m_position.z), currentTint);
			if (m_particles[i] != nullptr && m_Shape == "Cube")
				renderer->DrawCube(Vector3(m_particles[i]->m_position.x, m_particles[i]->m_position.y, m_particles[i]->m_position.z), currentTint);
		}
	}

	
}

void ParticleEmitter::RenderParticles(Renderer* renderer)
{
	for (unsigned int i = 0; i < m_particles.size(); i++)
	{
		Rgba currentTint = m_tint;
		currentTint.a = (unsigned char) RangeMapFloat((float)m_particles[i]->m_lifetime, 0.f, (float)initalLiveTime, 0.f, 255.f);

		if (m_particles[i] != nullptr && m_Shape == "Circle")
			renderer->drawCircle(5, currentTint, Vector2(m_particles[i]->m_position.x, m_particles[i]->m_position.y));
		if (m_particles[i] != nullptr && m_Shape == "Cylinder")
			renderer->drawCylinder(m_particles[i]->m_position.x, m_particles[i]->m_position.y, currentTint);
		if (m_particles[i] != nullptr && m_Shape == "Line2D")
			renderer->DrawLine2D(Vector2(m_particles[i]->m_position.x, m_particles[i]->m_position.y), Vector2(m_particles[i]->m_position.x, m_particles[i]->m_position.y), currentTint, currentTint);
		if (m_particles[i] != nullptr && m_Shape == "Line3D")
			renderer->DrawLine3D(Vector3(m_particles[i]->m_position.x, m_particles[i]->m_position.y, m_particles[i]->m_position.z), Vector3(m_particles[i]->m_position.x, m_particles[i]->m_position.y, m_particles[i]->m_position.z), currentTint, currentTint);
		/*
		if (m_particles[i] != nullptr && m_Shape == "AABB2")
			renderer->DrawAABB2(5, currentTint, Vector2(m_particles[i]->m_position.x, m_particles[i]->m_position.y));
		if (m_particles[i] != nullptr && m_Shape == "Box")
			renderer->DrawBox(5, currentTint, Vector2(m_particles[i]->m_position.x, m_particles[i]->m_position.y));
		if (m_particles[i] != nullptr && m_Shape == "TexturedAABB2")
			renderer->DrawTexturedAABB2(5, currentTint, Vector2(m_particles[i]->m_position.x, m_particles[i]->m_position.y));
		*/
	}
}

//------------------------------------------------Tools-------------------------------------------------------------------
void ParticleEmitter::Stop()
{
	m_pauseSpawning = true;
}

void ParticleEmitter::DestroyEffectOnFinish()
{
	m_finishedSpawning = true;
}

void ParticleEmitter::DestroyImmediate()
{
	m_finishedSpawning = true;

	DeleteCurrentParticles();
}

void ParticleEmitter::Play()
{
	m_pauseSpawning = false;
}

void ParticleEmitter::Reset()
{
	DeleteCurrentParticles();

	m_TimeToLive = initalLiveTime;
}

void ParticleEmitter::Prewarm(float prewarmAmount)
{
	float timer = 0;

	if (prewarmAmount > .1f)
		timer = .1f;
	else
	{
		timer = prewarmAmount;
	}
	while (timer != prewarmAmount)
	{
		Update(timer);

		if (prewarmAmount - timer > .1f)
			timer += .1f;
		else
		{
			timer = prewarmAmount - timer;
			Update(timer);
			return;
		}
	}
}

bool ParticleEmitter::IsFinished()
{
	if (m_finishedSpawning == false)
		return false;

	for (unsigned int i = 0; i < m_particles.size(); i++)
	{
		if (m_particles[i] != nullptr)
		{
			if (m_particles[i]->m_lifetime > 0)
				return false;
		}
	}
	return true;
}

void ParticleEmitter::DeleteCurrentParticles()
{
	for (unsigned int i = 0; i < m_particles.size(); i++)
	{
		delete m_particles[i];
		m_particles[i] = nullptr;
	}
}
