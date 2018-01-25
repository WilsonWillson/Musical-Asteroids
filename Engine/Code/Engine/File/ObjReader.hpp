#pragma once
#include <vector>
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/File/FileUtils.hpp"
#include "Engine/DX11/DX11Texture2D.hpp"
#include "Engine/RHI/ShaderProgram.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
#include <regex>
#include<map>

class ObjReader
{
public:
	ObjReader(char const* objectName, bool skipLines, bool invertAxis, DX11Texture2D* normal, DX11Texture2D* diffuse, ShaderProgram* shader);
	~ObjReader();

	void ReadObjFile(char const* objectName);
	void ReadObjFileNoSkipLine(char const* objectName);

public:
	DX11Texture2D*				m_NormalTexture;
	DX11Texture2D*				m_DiffuseTexture;
	ShaderProgram*				m_mainShader;
	std::vector<Vector3>		m_pointsVertexBuffer;
	std::vector<Vector3>		m_facesIndexBuffer;
	std::vector<Vector4>		m_4pointsVertexBuffer;
	std::vector<Vector4>		m_4facesIndexBuffer;
	std::string					m_objectName;
	bool						m_invertAxis;
};