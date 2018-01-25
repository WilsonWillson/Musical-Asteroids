#include "Engine/File/ObjReader.hpp"

ObjReader::ObjReader(char const* objectName, bool skipLines, bool invertAxis, DX11Texture2D* normal, DX11Texture2D* diffuse, ShaderProgram* shader)
{
	if (skipLines)
		ReadObjFile(objectName);
	else
		ReadObjFileNoSkipLine(objectName);

	m_invertAxis = invertAxis;
	m_NormalTexture = normal;
	m_DiffuseTexture = diffuse;
	m_mainShader = shader;
}

ObjReader::~ObjReader()
{
	delete m_NormalTexture;
	delete m_DiffuseTexture;
	delete m_mainShader;
}

void ObjReader::ReadObjFile(char const* objectName)
{
	int counter = 0;
	std::string c = ""; // read chars
	std::stringstream ssdata;

	size_t sourceSize = 0U;
	void* fileObj = FileReadToBuffer(objectName, &sourceSize);
	ssdata.write((char*)fileObj, sourceSize);

	std::getline(ssdata, c);
	while (!ssdata.eof())
	{
		counter++;
		std::getline(ssdata, c);
		char firstletter = c[0];
		//Name stuff
		if (firstletter == 103)
		{
			c.erase(0, 2);
			m_objectName = c;
		}

		//Vertexs
		if (firstletter == 118)
		{
			c.erase(0, 3);

			Vector3 temp;
			std::string delimiter = " ";

			size_t pos = 0;
			std::string token;
			std::string::size_type sz;

			pos = c.find(0x20);
			std::string	tokenX = c.substr(0, pos);
			temp.x = std::stof(tokenX, &sz);
			c.erase(0, pos + delimiter.length());

			pos = c.find(0x20);
			c.erase(0, pos + delimiter.length());

			pos = c.find(0x20);
			std::string tokenY = c.substr(0, pos);
			temp.y = std::stof(tokenY, &sz);
			c.erase(0, pos + delimiter.length());

			pos = c.find(0x20);
			c.erase(0, pos + delimiter.length());

			pos = c.find(0x20);
			std::string tokenZ = c.substr(0, pos);
			temp.z = std::stof(tokenZ, &sz);

			m_pointsVertexBuffer.push_back(temp);
		}

		if (counter == 953)
			counter = 0;
		//Faces
		if (firstletter == 102)
		{
			c.erase(0, 3);

			Vector3 temp;
			std::string delimiter = " ";

			size_t pos = 0;
			std::string token;
			std::string::size_type sz;

			pos = c.find(0x20);
			std::string	tokenX = c.substr(0, pos);
			temp.x = std::stof(tokenX, &sz);
			c.erase(0, pos + delimiter.length());

			pos = c.find(0x20);
			c.erase(0, pos + delimiter.length());

			pos = c.find(0x20);
			std::string tokenY = c.substr(0, pos);
			temp.y = std::stof(tokenY, &sz);
			c.erase(0, pos + delimiter.length());

			pos = c.find(0x20);
			c.erase(0, pos + delimiter.length());

			pos = c.find(0x20);
			std::string tokenZ = c.substr(0, pos);
			temp.z = std::stof(tokenZ, &sz);

			m_facesIndexBuffer.push_back(temp);
		}
	}
}

void ObjReader::ReadObjFileNoSkipLine(char const* objectName)
{
	int counter = 0;
	std::string c = ""; // read chars
	std::stringstream ssdata;

	size_t sourceSize = 0U;
	void* fileObj = FileReadToBuffer(objectName, &sourceSize);
	ssdata.write((char*)fileObj, sourceSize);

	std::getline(ssdata, c);
	while (!ssdata.eof())
	{
		counter++;
		std::getline(ssdata, c);
		char firstletter = c[0];
		//Name stuff
		if (firstletter == 103)
		{
			c.erase(0, 2);
			m_objectName = c;
		}

		//Vertexs
		if (firstletter == 118)
		{
			c.erase(0, 3);

			Vector3 temp;
			std::string delimiter = " ";

			size_t pos = 0;
			std::string token;
			std::string::size_type sz;

			pos = c.find(0x20);
			std::string	tokenX = c.substr(0, pos);
			temp.x = std::stof(tokenX, &sz);
			c.erase(0, pos + delimiter.length());

			//pos = c.find(0x20);
			//c.erase(0, pos + delimiter.length());

			pos = c.find(0x20);
			std::string tokenY = c.substr(0, pos);
			temp.y = std::stof(tokenY, &sz);
			c.erase(0, pos + delimiter.length());

			//pos = c.find(0x20);
			//c.erase(0, pos + delimiter.length());

			pos = c.find(0x20);
			std::string tokenZ = c.substr(0, pos);
			temp.z = std::stof(tokenZ, &sz);

			m_pointsVertexBuffer.push_back(temp);
		}

		if (counter == 953)
			counter = 0;
		//Faces
		if (firstletter == 102)
		{
			c.erase(0, 2);

			Vector3 temp;
			std::string delimiter = " ";

			size_t pos = 0;
			std::string token;
			std::string::size_type sz;

			pos = c.find(0x20);
			std::string	tokenX = c.substr(0, pos);
			temp.x = std::stof(tokenX, &sz);
			c.erase(0, pos + delimiter.length());

			pos = c.find(0x20);
			c.erase(0, pos + delimiter.length());

			pos = c.find(0x20);
			std::string tokenY = c.substr(0, pos);
			temp.y = std::stof(tokenY, &sz);
			c.erase(0, pos + delimiter.length());

			pos = c.find(0x20);
			c.erase(0, pos + delimiter.length());

			pos = c.find(0x20);
			std::string tokenZ = c.substr(0, pos);
			temp.z = std::stof(tokenZ, &sz);

			m_facesIndexBuffer.push_back(temp);
		}
	}
}
