#pragma once
#include "Engine/Core/StructsAndDefines.hpp"
#include <vector>
class Mesh
{

public:
	Mesh();
	~Mesh();

public:
	std::vector<vertex_t> m_verts;
};