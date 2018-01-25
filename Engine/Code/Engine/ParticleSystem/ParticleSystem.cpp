#include "Engine/ParticleSystem/ParticleSystem.hpp"
#include "ThirdParty/XMLParserUserTools.hpp"
#include "Engine/RHI/SimpleRenderer.hpp"
#include "Engine/Renderer/Renderer.hpp"

ParticleSystem::ParticleSystem()
{

}

ParticleSystem::~ParticleSystem()
{

}

//----------------------Update---------------------------------
void ParticleSystem::UpdateSystem(float deltaseconds)
{
	for (unsigned int i = 0; i < m_particleEmitters.size(); i++)
	{
		if (m_particleEmitters[i] != nullptr)
		{
			m_particleEmitters[i]->Update(deltaseconds);

			if (m_particleEmitters[i]->IsFinished() == true)
			{

				delete m_particleEmitters[i];
				m_particleEmitters[i] = nullptr;
			}
		}
	}
}

//--------------------------Add and Delete Particles------------------------
void ParticleSystem::AddParticleEmitter(std::string name, Vector4 position, Vector4 orientation, float timeToLive, float scale, bool doesItLoop, float prewarmAmount)
{
	XMLNode xMainNode = XMLNode::openFileHelper("Data/XML/Particles.xml", "Particles");

	//int AmountOfCharacterDefinitions = xMainNode.nChildNode("Particle");

	XMLNode childNode = FindParticleNodeBasedOffName(name, xMainNode);
	int step = FindChildNumberBasedOffName(name, xMainNode);


	ParticleEmitter* newEmitter = new ParticleEmitter();

	int amountOfAttributesOnDefinition = childNode.nAttribute();
	for (int attributeStep = 0; attributeStep < amountOfAttributesOnDefinition; attributeStep++)
	{
		std::string attributeName = childNode.getAttributeName(attributeStep);

		if ("Name" == attributeName)
			newEmitter->m_name = ParseXmlAttribute(xMainNode, "Particle", step, attributeStep, "invalidName");

		if ("Velocity" == attributeName)
			newEmitter->m_velocity = ParseXmlAttribute(xMainNode, "Particle", step, attributeStep, Vector4(0,0,0,0));

		if ("Tint" == attributeName)
			newEmitter->m_tint = ParseXmlAttribute(xMainNode, "Particle", step, attributeStep, Rgba(255, 255, 255, 255));

		if ("SpawnRate" == attributeName)
			newEmitter->m_SpawnRate = ParseXmlAttribute(xMainNode, "Particle", step, attributeStep, 1.0f);

		if ("SpawnBurst" == attributeName)
			newEmitter->m_SpawnBurst = ParseXmlAttribute(xMainNode, "Particle", step, attributeStep, 1.0f);

		if ("Shape" == attributeName)
			newEmitter->m_Shape = ParseXmlAttribute(xMainNode, "Particle", step, attributeStep, "line");

		if ("blend" == attributeName)
			newEmitter->m_blendMode = ParseXmlAttribute(xMainNode, "Particle", step, attributeStep, "line");

	}

	newEmitter->m_position = position;
	newEmitter->m_orientation = orientation;
	newEmitter->m_TimeToLive = timeToLive;
	newEmitter->initalLiveTime = timeToLive;
	newEmitter->m_Scale = scale;
	newEmitter->m_isLooping = doesItLoop;

	newEmitter->Prewarm(prewarmAmount);

	m_particleEmitters.push_back(newEmitter);
}

void ParticleSystem::DeleteParticleSystem(std::string nameOfParticle)
{
	for (unsigned int i = 0; i < m_particleEmitters.size(); i++)
	{
		if (nameOfParticle == m_particleEmitters[i]->m_name)
		{
			m_particleEmitters[i]->DestroyEffectOnFinish();
			return;
		}
	}
	return;
}

//---------------------------------Rendering------------------------------
void ParticleSystem::RenderSystemDX11(SimpleRenderer* renderer, Camera* camera, matrix_buffer_t matrix_data)
{
	for (unsigned int i = 0; i < m_particleEmitters.size(); i++)
	{
		if (m_particleEmitters[i] != nullptr)
		{
			renderer->setBillboardProjection(camera->m_pos, camera->m_pitchDegreesAboutX, camera->m_yawDegreesAboutY, camera->m_rollDegreesAboutZ);
			m_particleEmitters[i]->RenderParticles(renderer);
			matrix_data.model.MakeIdentity();

			//Render Emitter to show where it spawns from
			if (m_particleEmitters[i]->m_debugRenderPosition == true)
			{
				renderer->setBillboardProjection(camera->m_pos, camera->m_pitchDegreesAboutX, camera->m_yawDegreesAboutY, camera->m_rollDegreesAboutZ);
				Vector3 emitterPosition = Vector3(m_particleEmitters[i]->m_position.x, m_particleEmitters[i]->m_position.y, m_particleEmitters[i]->m_position.z);
				renderer->drawPlane(emitterPosition, Rgba(255, 105, 180, 255));
				matrix_data.model.MakeIdentity();
			}
		}
	}
}

void ParticleSystem::RenderSystemOpenGL(Renderer* renderer)
{
	for (unsigned int i = 0; i < m_particleEmitters.size(); i++)
	{
		m_particleEmitters[i]->RenderParticles(renderer);

		//Render Emitter to show where it spawns from
		if (m_particleEmitters[i]->m_debugRenderPosition == false)
		{
			renderer->drawCircle(5, m_particleEmitters[i]->m_tint, Vector2(m_particleEmitters[i]->m_position.x, m_particleEmitters[i]->m_position.y));
		}
	}
}


//--------------------------------Helpers----------------------------------------------------------------------------------------------------------

XMLNode ParticleSystem::FindParticleNodeBasedOffName(std::string value, XMLNode basicNode)
{
	int AmountOfCharacterDefinitions = basicNode.nChildNode("Particle");

	for (int tileStep = 0; tileStep < AmountOfCharacterDefinitions; tileStep++)
	{
		int amountOfAttributesOnDefinition = basicNode.getChildNode("Particle", tileStep).nAttribute();

		for (int attributeStep = 0; attributeStep < amountOfAttributesOnDefinition; attributeStep++)
		{
			std::string attributeName = basicNode.getChildNode("Particle", tileStep).getAttributeName(attributeStep);

			if ("Name" == attributeName)
			{
				std::string tempName = ParseXmlAttribute(basicNode, "Particle", tileStep, attributeStep, "invalidName");

				if (tempName == value)
					return basicNode.getChildNode("Particle", tileStep);
				else
					continue;
			}
		}
	}
	
	return basicNode;
}

int ParticleSystem::FindChildNumberBasedOffName(std::string value, XMLNode basicNode)
{
	int AmountOfCharacterDefinitions = basicNode.nChildNode("Particle");

	for (int tileStep = 0; tileStep < AmountOfCharacterDefinitions; tileStep++)
	{
		int amountOfAttributesOnDefinition = basicNode.getChildNode("Particle", tileStep).nAttribute();

		for (int attributeStep = 0; attributeStep < amountOfAttributesOnDefinition; attributeStep++)
		{
			std::string attributeName = basicNode.getChildNode("Particle", tileStep).getAttributeName(attributeStep);

			if ("Name" == attributeName)
			{
				std::string tempName = ParseXmlAttribute(basicNode, "Particle", tileStep, attributeStep, "invalidName");

				if (tempName == value)
					return tileStep;
				else
					continue;
			}
		}
	}

	return -1;
}
