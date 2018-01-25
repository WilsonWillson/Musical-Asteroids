#pragma once
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Renderer/BitMapFont.hpp"
#include <string>
#include <vector>



struct Vertex3_PCT
{
	Vertex3_PCT() {}
	Vertex3_PCT(const Vector3& m_position, const Rgba& m_color, const Vector2& m_texCoords)
	:m_position(m_position),
		m_color(m_color),
		m_texCoords(m_texCoords)
	{

	}
	
	Vector3 m_position;
	Rgba	m_color;
	Vector2	m_texCoords;
};

struct Vertex2D_PC
{
	Vector2		pos;
	Rgba		color;
};


struct Vertex2_PCT
{
	Vertex2_PCT() {}
	Vertex2_PCT(const Vector2& m_position, const Rgba& m_color, const Vector2& m_texCoords)
		:m_position(m_position),
		m_color(m_color),
		m_texCoords(m_texCoords)
	{

	}

	Vector2 m_position;
	Rgba	m_color;
	Vector2	m_texCoords;
};


struct Vertex3D_PC
{
	Vertex3D_PC() {}
	Vertex3D_PC(const Vector3& m_position, const Rgba& m_color)
		:m_position(m_position),
		m_color(m_color)
	{

	}


	Vector3		m_position;
	Rgba		m_color;
};


/////////////////////////////////////////////////////////////////////////////////////////////////
class Renderer
{
	friend SpriteSheet;
public:
	Renderer();
	~Renderer();

	void BeginLines();
	void BeginPolygon();
	void PushMatrix();
	void PopMatrix();
	void LoadIdentity();
	void DepthTest(bool depthTest);
	void SetLineThickness(float lineThickness);
	void Ortho(float left, float right, float bottom, float top, float zNear, float zFar);
	void setOrthoWithVectors(Vector2 mins, Vector2 maxs);
	void Enable3DOptions();
	void Disable3DOption();
	void EnableDepthMask();
	void DisableDepthMask();
	void SetPrespective(float fovYDegrees, float aspectRatio, float nearClipDist, float farClipDist);
	void DrawOriginLines();
	void DrawDebugPoints(std::vector<Vertex3_PCT> g_debugPoints, float pointSize, bool enableDepthTesting);
	void Translate3D(float screenshakeX, float screenshakeY);
	void Translate(Vector2 translate);
	void Translate(Vector3 translate);
	void GEnd();
	void GlBlendOn();
	void GlBlendOff();
	void ClearColor(float r, float g, float b, float a);
	void ClearColor(Rgba clearColors);
	void ClearColorBufferBit();
	void Color3f(float r, float g, float b);
	void Color4f(float r, float g, float b, float a);
	void Color3ub(float r, float g, float b);
	void Color4ub(float r, float g, float b, float a);
	void Vertex2f(float x, float y);
	void Translatef(float x, float y, float z);
	void Translate3D(float x, float y, float z);
	void Rotatef(float angle, float x, float y, float z);
	void Rotate3D(float angle, Vector3 position);
	void Scalef(float x, float y, float z);
	void DrawAABB2(const AABB2& bounds, const Rgba& color);
	void DrawNonTexturedAABB2(const AABB2& bounds, const Rgba& color);
	void DrawBox(const AABB2& bounds, const Rgba& color);
	void DrawLine2D(const Vector2& start, const Vector2& end, const Rgba& startColor, const Rgba& endColor);
	void DrawLine3D(const Vector3& start, const Vector3& end, const Rgba& startColor, const Rgba& endColor);
	//void drawCircle(float radius, Rgba edgeColor);
	void drawCircle(float radius, Rgba edgeColor, Vector2 pos);
	void drawCylinder(float m_cylinderRadius, float m_playerHeight, Rgba edgeColor);
	void RenderPoint(Vector2 currentPosition);

	void RenderPauseMenu();

	void DisableTexture();
	Texture* CreateOrGetTexture(const std::string& imageFilePath);
	BitMapFont* CreateOrGetFont(const std::string& bitmapFontName);
	void DrawTexturedAABB2(const AABB2& bounds, const Texture& texture, float orientation);
	void DrawTexturedAABB2(const AABB2& bounds, const Texture& texture, float orientation, const AABB2& texCoords, Rgba Color);
	void DrawTexturedQuad3D(const Vertex3_PCT& one, const Vertex3_PCT& two, const Vertex3_PCT& three, const Vertex3_PCT& four, const Texture& texture);

	void DrawText2D(const Vector2& startBottomLeft, const std::string& asciiText, float cellHeight, float cellWidth, const Rgba& tint, const BitMapFont* font = nullptr);
	void DrawText2D(const AABB2 boundsofBox, const std::string& asciiText, const Rgba& tint, const BitMapFont* font = nullptr);
	void DrawCenteredText2D(const Vector2& startCenter, const std::string& asciiText, float cellHeight, float cellWidth, const Rgba& tint, const BitMapFont* font = nullptr);

	void DrawDebugLine(const Vector2& start, const Vector2& end, float thickness, const Rgba& startColor, const Rgba& endColor);
	void DrawDebugAABB2(const AABB2& bounds, float thickness, const Rgba& fillColor, const Rgba& edgeColor);
	void DrawDebugCircle(const Vector2& midPosition, float radius, float lineThickness,	float sides, const Rgba& fillColor, const Rgba& edgeColor);

	void BindTexture(Texture& texture);
	void DrawVertexArray2D_PC(Vertex2D_PC* vertexArray, int numVertexes, unsigned int primativeDrawMode);
	void DrawVertexArray2D_PCT(Vertex2_PCT* vertexArray, int numVertexes, unsigned int primativeDrawMode);
	void DrawVertexArray3D_PC(Vertex3D_PC* vertexArray, int numVertexes, unsigned int primativeDrawMode);
	void DrawVertexArray3D_PCT(Vertex3_PCT* vertexArray, int numVertexes, unsigned int primativeDrawMode);

	
	void CheckError();
	void DrawVBO3D_PCT(unsigned int vboID, int numVertexes, unsigned int primativeDrawMode);
	unsigned int CreateVBO(); //returns vboID
	void UpdateVBO(unsigned int vboID, Vertex3_PCT* vertexArray, int numVertexes);
	void DeleteVBO(unsigned int vboID);

private:
	Texture* CreateTextureFromFile(const std::string& imageFilePath);
	BitMapFont* CreateBitMapFromFile(const std::string& imageFilePath);
	Texture* GetTexture(const std::string& imageFilePath);
	BitMapFont* GetFont(const std::string& imageFilePath);
	unsigned int CreateOpenGLTexture(unsigned char* imageTexelBytes, int width, int height, int bytesPerTexel);

private:
	std::vector< Texture* >		m_alreadyLoadedTextures;
};

