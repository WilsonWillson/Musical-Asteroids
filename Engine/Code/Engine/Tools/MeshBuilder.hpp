#pragma once
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/UVector4.hpp"
#include "Engine/Core/StructsAndDefines.hpp"
#include "Engine/RHI/RHI.hpp"
#include <vector>
class MeshBuilder
{

public:
	MeshBuilder();
	~MeshBuilder();
	void add_vertex(Vector3 position);
	void begin(ePrimitiveType param1, bool param2); //
	void clear();
	void end();
	void set_normal(Vector3 normal);

public:
	std::vector<vertex_t> m_meshVerts;
	std::vector<vertex_t> m_currentDrawCall;
	ePrimitiveType m_primative;
	Vector3 currentNormal;
	UVector4 currentBoneIndicies;
	Vector4 currentBoneWeights;
	int m_currentIndex;
	bool m_hasBegun;
};