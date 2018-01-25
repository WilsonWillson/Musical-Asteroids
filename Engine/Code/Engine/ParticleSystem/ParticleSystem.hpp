#pragma once
#include "ThirdParty/XMLParser/XMLParser.hpp"
#include "Engine/ParticleSystem/ParticleEmitter.hpp"
#include "Engine/Core/Camera.hpp"
#include "Engine/Math/Vector4.hpp"
#include <vector>
#include <string>


class SimpleRenderer;
struct matrix_buffer_t;

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void UpdateSystem(float deltaseconds);

	void RenderSystemDX11(SimpleRenderer* renderer, Camera* camera, matrix_buffer_t matrix_data);
	void RenderSystemOpenGL(Renderer* renderer);

	void AddParticleEmitter(std::string name, Vector4 position, Vector4 orientation, float timeToLive, float scale, bool doesItLoop, float prewarmAmount);
	void DeleteParticleSystem(std::string nameOfParticle);

private:
	XMLNode FindParticleNodeBasedOffName(std::string name, XMLNode basicNode);
	int FindChildNumberBasedOffName(std::string value, XMLNode basicNode);

public:
	std::vector<ParticleEmitter*> m_particleEmitters;
};