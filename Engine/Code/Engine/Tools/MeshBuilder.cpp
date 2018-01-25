#include "Engine/Tools/MeshBuilder.hpp"

MeshBuilder::MeshBuilder()
{
	m_currentDrawCall.clear();
	m_meshVerts.clear();
	m_hasBegun = false;
	m_currentIndex = 0;
}

MeshBuilder::~MeshBuilder()
{

}

void MeshBuilder::add_vertex(Vector3 position)
{
	vertex_t vertex;
	vertex.position = position;
	vertex.normal = currentNormal;
	vertex.boneWeights = currentBoneWeights;

	vertex.boneIndices = currentBoneIndicies;
	m_meshVerts.push_back(vertex);
}

void MeshBuilder::begin(ePrimitiveType primativeType, bool whoKnows)
{
	m_primative = primativeType;
	m_hasBegun = whoKnows;
}

void MeshBuilder::clear()
{
	m_currentDrawCall.clear();
	m_meshVerts.clear();
	m_hasBegun = false;
	m_currentIndex = 0;
}

void MeshBuilder::end()
{
	m_currentIndex = 0;
}

void MeshBuilder::set_normal(Vector3 normal)
{
	currentNormal = normal;
}

