#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library
#pragma comment(lib, "Glu32")// Link in the OpenGLU32.lib static library
#include "Engine/Core/Win32Includes.hpp"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ThirdParty/OpenGL/wglext.h" // Modern openGLHeader Files
#include "ThirdParty/OpenGL/glext.h" // Modern openGLHeader Files
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "ThirdParty/stb/stb_image.h"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Renderer/BitMapFont.hpp"


PFNGLGENBUFFERSPROC		glGenBuffers = nullptr;
PFNGLBINDBUFFERPROC		glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC		glBufferData = nullptr;

//PFNGLGENERATEMIPMAPPROC	glGenerateMipmap = nullptr;
PFNGLDELETEBUFFERSPROC	glDeleteBuffers = nullptr;

Renderer::Renderer()
{
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	//glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
}

Renderer::~Renderer()
{

}

void Renderer::BeginLines()
{
	glBegin(GL_LINES);
}

void Renderer::BeginPolygon()
{
	glBegin(GL_POLYGON);
}

void Renderer::PushMatrix()
{
	glPushMatrix();
}

void Renderer::PopMatrix()
{
	glPopMatrix();
}

void Renderer::LoadIdentity()
{
	glLoadIdentity();
}

void Renderer::DepthTest(bool depthTest)
{
	if (depthTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void Renderer::SetLineThickness(float lineThickness)
{
	glLineWidth(lineThickness);
}

void Renderer::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	glLoadIdentity();
	glOrtho(left, right, bottom, top, zNear, zFar);
}

void Renderer::setOrthoWithVectors(Vector2 mins, Vector2 maxs)
{
	glLoadIdentity();
	glOrtho(mins.x, maxs.x, mins.y, maxs.y, 0, 1);
}

void Renderer::Enable3DOptions()
{
	glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);
	glClearDepth(1.f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void Renderer::Disable3DOption()
{
	glDisable(GL_DEPTH_TEST);
}

void Renderer::EnableDepthMask()
{
	glDepthMask(GL_TRUE);
}

void Renderer::DisableDepthMask()
{
	glDepthMask(GL_FALSE);
}

void Renderer::SetPrespective(float fovYDegrees, float aspectRatio, float nearClipDist, float farClipDist)
{
	glLoadIdentity();
	gluPerspective(fovYDegrees, aspectRatio, nearClipDist, farClipDist);
}

void Renderer::DrawOriginLines()
{
	glLineWidth(3);
	DrawLine3D(Vector3(0.f, 0.f, 0.f), Vector3(1.f, 0.f, 0.f), Rgba(255, 0, 0, 255), Rgba(255, 0, 0, 255));
	DrawLine3D(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f), Rgba(0, 255, 0, 255), Rgba(0, 255, 0, 255));
	DrawLine3D(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f), Rgba(0, 0, 255, 255), Rgba(0, 0, 255, 255));
	
	glLineWidth(1);
	glDisable(GL_DEPTH_TEST);
	DrawLine3D(Vector3(0.f, 0.f, 0.f), Vector3(1.f, 0.f, 0.f), Rgba(255, 0, 0, 76), Rgba(255, 0, 0, 76));
	DrawLine3D(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f), Rgba(0, 255, 0, 76), Rgba(0, 255, 0, 76));
	DrawLine3D(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f), Rgba(0, 0, 255, 76), Rgba(0, 0, 255, 76));
}

void Renderer::DrawDebugPoints(std::vector<Vertex3_PCT> g_debugPoints, float pointSize, bool enableDepthTesting)
{
	Texture* texture3 = CreateOrGetTexture("Data/Images/default.png");
	glBindTexture(GL_TEXTURE_2D, texture3->m_textureID);
	
	glLineWidth(pointSize);
	if (enableDepthTesting == true)
		glEnable(GL_DEPTH_TEST);
	if (enableDepthTesting == false)
		glDisable(GL_DEPTH_TEST);
	//DrawVertexArray3D_PCT(&g_debugPoints[0], g_debugPoints.size(), 0x0001);
}

void Renderer::Translate3D(float screenshakeX, float screenshakeY)
{
	glTranslatef(screenshakeX, screenshakeY, 0);
}

void Renderer::Translate(Vector2 translate)
{
	glTranslatef(translate.x, translate.y, 0);
}

void Renderer::Translate(Vector3 translate)
{
	glTranslatef(translate.x, translate.y, translate.z);
}

void Renderer::GEnd()
{
	glEnd();
}
void Renderer::GlBlendOn()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}
void Renderer::GlBlendOff()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void Renderer::ClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::ClearColor(Rgba clearColors)
{
	float r, g, b, a;
	clearColors.GetAsFloats(r, g, b, a);
	glClearColor(r, g, b, a);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::ClearColorBufferBit()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Color3f(float r, float g, float b)
{
	glColor3f(r, g, b);
}

void Renderer::Color4f(float r, float g, float b, float a)
{
	glColor4f(r, g, b,a);
}

void Renderer::Color3ub(float r, float g, float b) {
	
	glColor3ub((GLubyte)r, (GLubyte)g, (GLubyte)b);
}

void Renderer::Color4ub(float r, float g, float b, float a) {

	glColor4ub((GLubyte)r, (GLubyte)g, (GLubyte)b, (GLubyte)a);
}

void Renderer::Vertex2f(float x, float y)
{
	glVertex2f(x, y);
}

void Renderer::Translatef(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

void Renderer::Translate3D(float x, float y, float z)
{
	glTranslatef(x, y, z);	
}

void Renderer::Rotatef(float angle, float x, float y, float z)
{
	glRotatef(angle, x, y, z);
}

void Renderer::Rotate3D(float angle, Vector3 position)
{
	glRotatef(angle, position.x, position.y, position.z);
}

void Renderer::Scalef(float x, float y, float z)
{
	glScalef(x, y, z);
}

void Renderer::DrawAABB2(const AABB2& bounds, const Rgba& color) 
{
	Texture* texture3 = CreateOrGetTexture("Data/Images/default.png");
	glBindTexture(GL_TEXTURE_2D, texture3->m_textureID);

	std::vector<Vertex3_PCT> vertexes;
	Vertex3_PCT vertex;
	vertex.m_color = color;
		
	vertex.m_position = Vector3(bounds.m_mins.x, bounds.m_mins.y, 0);
	vertexes.push_back(vertex);

	vertex.m_position = Vector3(bounds.m_maxs.x, bounds.m_mins.y, 0);
	vertexes.push_back(vertex);

	vertex.m_position = Vector3(bounds.m_maxs.x, bounds.m_maxs.y, 0);
	vertexes.push_back(vertex);

	vertex.m_position = Vector3(bounds.m_mins.x, bounds.m_maxs.y, 0);
	vertexes.push_back(vertex);

	DrawVertexArray3D_PCT(&vertexes[0], 4, GL_QUADS);
}


void Renderer::DrawNonTexturedAABB2(const AABB2& bounds, const Rgba& color)
{
	//Texture* texture3 = CreateOrGetTexture("Data/Images/default.png");
	//glBindTexture(GL_TEXTURE_2D, texture3->m_textureID);

	std::vector<Vertex3D_PC> vertexes;
	Vertex3D_PC vertex;
	vertex.m_color = color;

	vertex.m_position = Vector3(bounds.m_mins.x, bounds.m_mins.y, 0);
	vertexes.push_back(vertex);

	vertex.m_position = Vector3(bounds.m_maxs.x, bounds.m_mins.y, 0);
	vertexes.push_back(vertex);

	vertex.m_position = Vector3(bounds.m_maxs.x, bounds.m_maxs.y, 0);
	vertexes.push_back(vertex);

	vertex.m_position = Vector3(bounds.m_mins.x, bounds.m_maxs.y, 0);
	vertexes.push_back(vertex);

	DrawVertexArray3D_PC(&vertexes[0], 4, GL_QUADS);
}

void Renderer::DrawBox(const AABB2& bounds, const Rgba& color)
{
	glLineWidth(10);
	DrawLine2D(bounds.m_mins, Vector2(bounds.m_mins.x, bounds.m_maxs.y), color, color);
	DrawLine2D(Vector2(bounds.m_mins.x, bounds.m_maxs.y), bounds.m_maxs, color, color);
	DrawLine2D(bounds.m_maxs, Vector2(bounds.m_maxs.x, bounds.m_mins.y), color, color);
	DrawLine2D(Vector2(bounds.m_maxs.x, bounds.m_mins.y), bounds.m_mins, color, color);
	glLineWidth(1);
}

//-------------------Based on Code by Squirrel Eiserloh----------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Texture* Renderer::CreateOrGetTexture(const std::string& imageFilePath)
{
	// Try to find that texture from those already loaded
	Texture* texture = GetTexture(imageFilePath);
	if (texture)
		return texture;

	texture = CreateTextureFromFile(imageFilePath);
	return texture;
}

//-----------------------------------------------------------------------------------------------
Texture* Renderer::CreateTextureFromFile(const std::string& imageFilePath)
{
	// Load image data
	int width = 0;
	int height = 0;
	int bytesPerTexel = 0;
	unsigned char* imageTexelBytes = stbi_load(imageFilePath.c_str(), &width, &height, &bytesPerTexel, 0);
	GUARANTEE_OR_DIE(imageTexelBytes != nullptr, Stringf("Failed to load image file \"%s\" - file not found!", imageFilePath.c_str()));
	GUARANTEE_OR_DIE(bytesPerTexel == 3 || bytesPerTexel == 4, Stringf("Failed to load image file \"%s\" - image had unsupported %i bytes per texel (must be 3 or 4)", imageFilePath.c_str(), bytesPerTexel));

	// Create texture on video card, and send image (texel) data
	unsigned int openGLTextureID = CreateOpenGLTexture(imageTexelBytes, width, height, bytesPerTexel);
	stbi_image_free(imageTexelBytes);

	// Create (new) a Texture object
	Texture* texture = new Texture();
	texture->m_textureID = openGLTextureID;
	texture->m_imageFilePath = imageFilePath;
	texture->m_texelDimensions.SetXY(width, height);

	m_alreadyLoadedTextures.push_back(texture);
	return texture;
}

//-----------------------------------------------------------------------------------------------
Texture* Renderer::GetTexture(const std::string& imageFilePath)
{
	if (m_alreadyLoadedTextures.empty())
		return nullptr;
	for (int textureIndex = 0; textureIndex < (int)m_alreadyLoadedTextures.size(); ++textureIndex)
	{
		if (m_alreadyLoadedTextures[textureIndex] != nullptr) {
			Texture* texture = m_alreadyLoadedTextures[textureIndex];
			if (imageFilePath == texture->m_imageFilePath)
				return texture;
		}
	}

	return nullptr;
}

//-----------------------------------------------------------------------------------------------
unsigned int Renderer::CreateOpenGLTexture(unsigned char* imageTexelBytes, int width, int height, int bytesPerTexel)
{
	// Create a texture ID (or "name" as OpenGL calls it) for this new texture
	unsigned int openGLTextureID = 0xFFFFFFFF;
	glGenTextures(1, &openGLTextureID);

	// Enable texturing
	glEnable(GL_TEXTURE_2D);

	// Tell OpenGL that our pixel data is single-byte aligned
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Tell OpenGL to bind (set) this as the currently active texture
	glBindTexture(GL_TEXTURE_2D, openGLTextureID);

	// Set texture clamp vs. wrap (repeat)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // one of: GL_CLAMP or GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // one of: GL_CLAMP or GL_REPEAT

																  // Set magnification (texel > pixel) and minification (texel < pixel) filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // one of: GL_NEAREST, GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // one of: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR

	GLenum bufferFormat = GL_RGBA; // the format our source pixel data is currently in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...
	if (bytesPerTexel == 3)
		bufferFormat = GL_RGB;

	GLenum internalFormat = bufferFormat; // the format we want the texture to me on the card; allows us to translate into a different texture format as we upload to OpenGL

	glTexImage2D(			// Upload this pixel data to our new OpenGL texture
		GL_TEXTURE_2D,		// Creating this as a 2d texture
		0,					// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
		internalFormat,		// Type of texel format we want OpenGL to use for this texture internally on the video card
		width,				// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,10], and B is the border thickness [0,1]
		height,				// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,10], and B is the border thickness [0,1]
		0,					// Border size, in texels (must be 0 or 1)
		bufferFormat,		// Pixel format describing the composition of the pixel data in buffer
		GL_UNSIGNED_BYTE,	// Pixel color components are unsigned bytes (one byte per color/alpha channel)
		imageTexelBytes);	// Location of the actual pixel data bytes/buffer

	CheckError();

	return openGLTextureID;
}


void Renderer::drawCylinder(float cylinderRadius, float playerHeight, Rgba edgeColor)
{
	Texture* texture3 = CreateOrGetTexture("Data/Images/default.png");
	glBindTexture(GL_TEXTURE_2D, texture3->m_textureID);
	
	glEnable(GL_DEPTH_TEST);
	glLineWidth(3);
	//Draw Bottom part of the Circle
	drawCircle(cylinderRadius, edgeColor, Vector2(0,0));
	//Translate up 
	Translate(Vector3(0, 0, playerHeight));
	

	const int NUM_SIDES = 48;
	const float DEGREES_PER_SIDE = 360.f / (float)NUM_SIDES;

	std::vector<Vertex3_PCT> vertexes;
	Vertex3_PCT vertex;
	vertex.m_color = edgeColor;

	for (int sideIndex = 0; sideIndex < NUM_SIDES; ++sideIndex)
	{
		float angleDegrees = DEGREES_PER_SIDE * (float)sideIndex;
		float x = cylinderRadius * CosDegrees(angleDegrees);
		float y = cylinderRadius * SinDegrees(angleDegrees);
		DrawLine3D(Vector3(x, y, 0), Vector3(x, y, -playerHeight), edgeColor, edgeColor);
		vertex.m_position = Vector3(x, y, 0);
		vertexes.push_back(vertex);
	}

	DrawVertexArray3D_PCT(&vertexes[0], NUM_SIDES, GL_LINE_LOOP);
	glDisable(GL_DEPTH_TEST);
	glLineWidth(1);
	//Draw Bottom part of the Circle
	drawCircle(cylinderRadius, edgeColor, Vector2(0,0));

	for (int sideIndex = 0; sideIndex < NUM_SIDES; ++sideIndex)
	{
		float angleDegrees = DEGREES_PER_SIDE * (float)sideIndex;
		float x = cylinderRadius * CosDegrees(angleDegrees);
		float y = cylinderRadius * SinDegrees(angleDegrees);
		DrawLine3D(Vector3(x, y, 0), Vector3(x, y, -playerHeight), edgeColor, edgeColor);
		vertex.m_position = Vector3(x, y, 0);
		vertexes.push_back(vertex);
	}

	DrawVertexArray3D_PCT(&vertexes[0], NUM_SIDES, GL_LINE_LOOP);
}

void Renderer::BindTexture(Texture& texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.m_textureID);
}

void Renderer::drawCircle(float radius, Rgba edgeColor, Vector2 pos)
{
	Texture* texture3 = CreateOrGetTexture("Data/Images/default.png");
	glBindTexture(GL_TEXTURE_2D, texture3->m_textureID);
	
	const int NUM_SIDES = 48;
	const float DEGREES_PER_SIDE = 360.f / (float)NUM_SIDES;

	std::vector<Vertex3_PCT> vertexes;
	Vertex3_PCT vertex;
	vertex.m_color = edgeColor;

	for (int sideIndex = 0; sideIndex < NUM_SIDES; ++sideIndex)
	{
		float angleDegrees = DEGREES_PER_SIDE * (float)sideIndex;
		float x = radius * CosDegrees(angleDegrees) + pos.x;
		float y = radius * SinDegrees(angleDegrees) + pos.y;
		vertex.m_position = Vector3(x,y, 0);
		vertexes.push_back(vertex);
	}

	DrawVertexArray3D_PCT(&vertexes[0], NUM_SIDES, GL_LINE_LOOP);
}

//-----------------------------------------------------------------------------------------------
void Renderer::DrawTexturedAABB2(const AABB2& bounds, const Texture& texture, float orientationDegrees)
{
	AABB2 texCoords(0.f, 0.f, 1.f, 1.f);
	DrawTexturedAABB2(bounds, texture, orientationDegrees, texCoords, Rgba(255,255,255,255));
}


void Renderer::DrawTexturedAABB2(const AABB2& bounds, const Texture& texture, float orientation, const AABB2& texCoords, Rgba color)
{
	if (&texture == nullptr)
		return;

	glBindTexture(GL_TEXTURE_2D, texture.m_textureID);


	Vertex3_PCT vertexes[4];
	//Rgba color = Rgba(255, 255, 255, 255);


	Rotatef(orientation, 0.f, 0.f, 1.f);

	vertexes[0].m_position = Vector3(bounds.m_mins.x, bounds.m_mins.y, 0);
	vertexes[0].m_color = color;
	vertexes[0].m_texCoords = Vector2(texCoords.m_mins.x, texCoords.m_maxs.y);

	vertexes[1].m_position = Vector3(bounds.m_maxs.x, bounds.m_mins.y, 0);
	vertexes[1].m_color = color;
	vertexes[1].m_texCoords = Vector2(texCoords.m_maxs.x, texCoords.m_maxs.y);


	vertexes[2].m_position = Vector3(bounds.m_maxs.x, bounds.m_maxs.y, 0);
	vertexes[2].m_color = color;
	vertexes[2].m_texCoords = Vector2(texCoords.m_maxs.x, texCoords.m_mins.y);

	vertexes[3].m_position = Vector3(bounds.m_mins.x, bounds.m_maxs.y, 0);
	vertexes[3].m_color = color;
	vertexes[3].m_texCoords = Vector2(texCoords.m_mins.x, texCoords.m_mins.y);
	
	/*Vertex3_PCT one(Vector3(bounds.m_mins.x, bounds.m_mins.y, 0), color, Vector2(texCoords.m_mins.x, texCoords.m_maxs.y));
	Vertex3_PCT two(Vector3(bounds.m_maxs.x, bounds.m_mins.y, 0), color, Vector2(texCoords.m_maxs.x, texCoords.m_maxs.y));
	Vertex3_PCT three(Vector3(bounds.m_maxs.x, bounds.m_maxs.y, 0), color, Vector2(texCoords.m_maxs.x, texCoords.m_mins.y));
	Vertex3_PCT four(Vector3(bounds.m_mins.x, bounds.m_maxs.y, 0), color, Vector2(texCoords.m_mins.x, texCoords.m_mins.y));

	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);*/

	DrawVertexArray3D_PCT(&vertexes[0], 4, GL_QUADS);
}

void Renderer::DrawTexturedQuad3D(const Vertex3_PCT& one, const Vertex3_PCT& two, const Vertex3_PCT& three, const Vertex3_PCT& four, const Texture& texture)
{
	glBindTexture(GL_TEXTURE_2D, texture.m_textureID);
	std::vector<Vertex3_PCT> vertexes;

	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);

	DrawVertexArray3D_PCT(&vertexes[0], 2, GL_QUADS);
}

void Renderer::DrawText2D(const Vector2& startBottomLeft, const std::string& asciiText, float cellHeight, float cellWidth, const Rgba& tint, const BitMapFont* font /*= nullptr*/)
{
	float letterWidth = cellWidth / asciiText.length();
	glColor3f(tint.r, tint.g, tint.b);
	for (unsigned int i = 0; i < asciiText.length(); i++)
	{
		AABB2 bounds(startBottomLeft.x + letterWidth * i, startBottomLeft.y, startBottomLeft.x + letterWidth * (i+1), startBottomLeft.y + cellHeight);
		char x = asciiText.at(i);
		AABB2 textCoords = font->GetTexCoordsForGlyph(int(x));
		Texture*  tempTex = font->m_spriteSheet->m_spriteSheetTexture;

		DrawTexturedAABB2(bounds, *tempTex, 0, textCoords, tint);
	}
	glColor3f(255, 255, 255);
}

void Renderer::DrawText2D(const AABB2 boundsofBox, const std::string& asciiText, const Rgba& tint, const BitMapFont* font /*= nullptr*/)
{
	float cellWidth = boundsofBox.m_maxs.x - boundsofBox.m_mins.x;
	float cellHeight = boundsofBox.m_maxs.y - boundsofBox.m_mins.y;
	
	float letterWidth = cellWidth / asciiText.length();
	glColor3f(tint.r, tint.g, tint.b);
	for (unsigned int i = 0; i < asciiText.length(); i++)
	{
		AABB2 bounds(boundsofBox.m_mins.x + letterWidth * i, boundsofBox.m_mins.y, boundsofBox.m_mins.x + letterWidth * (i + 1), boundsofBox.m_mins.y + cellHeight);
		char x = asciiText.at(i);
		AABB2 textCoords = font->GetTexCoordsForGlyph(int(x));
		Texture*  tempTex = font->m_spriteSheet->m_spriteSheetTexture;

		DrawTexturedAABB2(bounds, *tempTex, 0, textCoords, tint);
	}
	glColor3f(255, 255, 255);
}

void Renderer::DrawCenteredText2D(const Vector2& startCenter, const std::string& asciiText, float cellHeight, float cellWidth, const Rgba& tint, const BitMapFont* font /*= nullptr*/)
{
	float letterWidth = cellWidth / asciiText.length();

	//first find length of word
	Vector2 desiredBottomLeftStart = Vector2(startCenter.x - cellWidth / 2,startCenter.y - cellHeight/2);

	glColor3f(tint.r, tint.g, tint.b);
	for (unsigned int i = 0; i < asciiText.length(); i++)
	{
		AABB2 bounds(desiredBottomLeftStart.x + letterWidth * i, desiredBottomLeftStart.y, desiredBottomLeftStart.x + letterWidth * (i + 1), desiredBottomLeftStart.y + cellHeight);
		char x = asciiText.at(i);
		AABB2 textCoords = font->GetTexCoordsForGlyph(int(x));
		Texture*  tempTex = font->m_spriteSheet->m_spriteSheetTexture;

		DrawTexturedAABB2(bounds, *tempTex, 0, textCoords, tint);
	}
	glColor3f(255, 255, 255);
}

void Renderer::DrawDebugLine(const Vector2& start, const Vector2& end, float thickness, const Rgba& startColor, const Rgba& endColor)
{
	SetLineThickness(thickness);
	Vector3 newStart = Vector3(start.x, start.y, 0);
	Vector3 newEnd = Vector3(end.x, end.y, 0);
	Texture* texture3 = CreateOrGetTexture("Data/Images/default.png");
	glBindTexture(GL_TEXTURE_2D, texture3->m_textureID);

	Vertex3_PCT vertexes[2];
	vertexes[0].m_position = newStart;
	vertexes[0].m_color = startColor;
	vertexes[1].m_position = newEnd;
	vertexes[1].m_color = endColor;

	DrawVertexArray3D_PCT(&vertexes[0], 2, GL_LINES);

	SetLineThickness(1.0);
}

void Renderer::DrawDebugAABB2(const AABB2& bounds, float thickness, const Rgba& fillColor, const Rgba& edgeColor)
{
	SetLineThickness(thickness);
	std::vector<Vertex3D_PC> vertexes;
	Vertex3D_PC vertex;
	vertex.m_color = edgeColor;

	vertex.m_position = Vector3(bounds.m_mins.x, bounds.m_mins.y, 0);
	vertexes.push_back(vertex);

	vertex.m_position = Vector3(bounds.m_maxs.x, bounds.m_mins.y, 0);
	vertexes.push_back(vertex);

	vertex.m_position = Vector3(bounds.m_maxs.x, bounds.m_maxs.y, 0);
	vertexes.push_back(vertex);

	vertex.m_position = Vector3(bounds.m_mins.x, bounds.m_maxs.y, 0);
	vertexes.push_back(vertex);

	DrawVertexArray3D_PC(&vertexes[0], 4, GL_QUADS);


	Vector2 SquareWidthLength;
	SquareWidthLength.x = bounds.m_maxs.x - bounds.m_mins.x;
	SquareWidthLength.y = bounds.m_maxs.y - bounds.m_mins.y;


	std::vector<Vertex3D_PC> vertexes2;

	vertex.m_color = fillColor;

	vertex.m_position = Vector3(bounds.m_mins.x + SquareWidthLength.x/32, bounds.m_mins.y + SquareWidthLength.y / 32, 0);
	vertexes2.push_back(vertex);

	vertex.m_position = Vector3(bounds.m_maxs.x - SquareWidthLength.x /32, bounds.m_mins.y + SquareWidthLength.y / 32, 0);
	vertexes2.push_back(vertex);

	vertex.m_position = Vector3(bounds.m_maxs.x - SquareWidthLength.x / 32, bounds.m_maxs.y - SquareWidthLength.y / 32, 0);
	vertexes2.push_back(vertex);

	vertex.m_position = Vector3(bounds.m_mins.x + SquareWidthLength.x / 32, bounds.m_maxs.y - SquareWidthLength.y / 32, 0);
	vertexes2.push_back(vertex);
	DrawVertexArray3D_PC(&vertexes2[0], 4, GL_QUADS);

	SetLineThickness(1.0);
}

void Renderer::DrawDebugCircle(const Vector2& midPosition, float radius, float lineThickness, float sides, const Rgba& fillColor, const Rgba& edgeColor)
{
	SetLineThickness(lineThickness);
	Texture* texture3 = CreateOrGetTexture("Data/Images/default.png");
	glBindTexture(GL_TEXTURE_2D, texture3->m_textureID);

	const float DEGREES_PER_SIDE = 360.f / (float)sides;

	std::vector<Vertex3_PCT> vertexes;
	Vertex3_PCT vertex;
	vertex.m_color = edgeColor;

	for (int sideIndex = 0; sideIndex < sides; ++sideIndex)
	{
		float angleDegrees = DEGREES_PER_SIDE * (float)sideIndex;
		float x = radius * CosDegrees(angleDegrees) + midPosition.x;
		float y = radius * SinDegrees(angleDegrees) + midPosition.y;
		vertex.m_position = Vector3(x, y, 0);
		vertexes.push_back(vertex);
	}

	DrawVertexArray3D_PCT(&vertexes[0], (int)sides, GL_TRIANGLE_FAN);

	vertex.m_color =  fillColor;

	std::vector<Vertex3_PCT> vertexes2;
	for (int sideIndex = 0; sideIndex < sides; ++sideIndex)
	{
		float angleDegrees = DEGREES_PER_SIDE * (float)sideIndex;
		float x = radius * .75f * CosDegrees(angleDegrees) + midPosition.x;
		float y = radius* .75f  * SinDegrees(angleDegrees) + midPosition.y;
		vertex.m_position = Vector3(x, y, 0);
		vertexes2.push_back(vertex);
	}

	DrawVertexArray3D_PCT(&vertexes2[0], (int)sides, GL_TRIANGLE_FAN);

	SetLineThickness(1.0);
}

void Renderer::DrawVertexArray2D_PC(Vertex2D_PC* vertexArray, int numVertexes, unsigned int primativeDrawMode)
{
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, sizeof(Vertex2D_PC), &vertexArray->pos);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex2D_PC), &vertexArray->color);

	glDrawArrays(primativeDrawMode, 0, numVertexes);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void Renderer::DrawVertexArray2D_PCT(Vertex2_PCT* vertexArray, int numVertexes, unsigned int primativeDrawMode)
{
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, sizeof(Vertex2_PCT), &vertexArray->m_position);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex2_PCT), &vertexArray->m_color);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex2_PCT), &vertexArray->m_texCoords);
	
	glDrawArrays(primativeDrawMode, 0, numVertexes);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisable(GL_TEXTURE_2D);
}


void Renderer::DrawVertexArray3D_PC(Vertex3D_PC* vertexArray, int numVertexes, unsigned int primativeDrawMode)
{
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3D_PC), &vertexArray->m_position);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex3D_PC), &vertexArray->m_color);

	glDrawArrays(primativeDrawMode, 0, numVertexes);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void Renderer::DrawVertexArray3D_PCT(Vertex3_PCT* vertexArray, int numVertexes, unsigned int primativeDrawMode)
{
	CheckError();
		glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex3_PCT), &vertexArray->m_position);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex3_PCT), &vertexArray->m_color);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3_PCT), &vertexArray->m_texCoords);

		glDrawArrays(primativeDrawMode, 0, numVertexes);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		CheckError();
	}

void Renderer::CheckError()
{
	GLenum errorwehave = glGetError();

	if (errorwehave != GL_NO_ERROR)
	{

		GUARANTEE_RECOVERABLE(errorwehave == GL_NO_ERROR, "Oh no we have a problem");

	}
}


void Renderer::DrawVBO3D_PCT(unsigned int vboID, int numVertexes, unsigned int primativeDrawMode)
{
	//glPushMatrix();

	//glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3_PCT), (const GLvoid*)offsetof(Vertex3_PCT, m_position));
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex3_PCT), (const GLvoid*)offsetof(Vertex3_PCT, m_color));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3_PCT), (const GLvoid*)offsetof(Vertex3_PCT, m_texCoords));

	glDrawArrays(primativeDrawMode, 0, numVertexes);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glPopMatrix();
}


unsigned int Renderer::CreateVBO()
{
	unsigned int vboID;
	glGenBuffers(1, &vboID);
	return vboID;
}

void Renderer::UpdateVBO(unsigned int vboID, Vertex3_PCT* vertexArray, int numVertexes)
{
	size_t vertexArrayNumBytes = numVertexes * sizeof(Vertex3_PCT);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, vertexArrayNumBytes,vertexArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::DeleteVBO(unsigned int vboID)
{
	glDeleteBuffers(1, &vboID);
}


void Renderer::DrawLine2D(const Vector2& start, const Vector2& end, const Rgba& startColor, const Rgba& endColor)
{
	CheckError();
	Vector3 newStart = Vector3(start.x, start.y, 0);
	Vector3 newEnd = Vector3(end.x, end.y, 0);
	Texture* texture3 = CreateOrGetTexture("Data/Images/default.png");
	glBindTexture(GL_TEXTURE_2D, texture3->m_textureID);
	
	Vertex3D_PC vertexes[2];

	vertexes[0].m_position = newStart;
	vertexes[0].m_color = startColor;
	vertexes[1].m_position = newEnd;
	vertexes[1].m_color = endColor;

	DrawVertexArray3D_PC(&vertexes[0], 2, GL_LINES);
}

void Renderer::DrawLine3D(const Vector3& start, const Vector3& end, const Rgba& startColor, const Rgba& endColor)
{
	Texture* texture3 = CreateOrGetTexture("Data/Images/default.png");
	glBindTexture(GL_TEXTURE_2D, texture3->m_textureID);

	Vertex3_PCT vertexes[2];
	vertexes[0].m_position = start;
	vertexes[0].m_color = startColor;
	vertexes[0].m_texCoords = Vector2(0, 0);
	vertexes[1].m_position = end;
	vertexes[1].m_color = endColor;
	vertexes[1].m_texCoords = Vector2(1, 1);

	DrawVertexArray3D_PCT(&vertexes[0], 2, GL_LINES);
}

void Renderer::RenderPauseMenu()
{
	/*glDisable(GL_TEXTURE_2D);
	Color4f(0.f, 0.f, 0.f, .4f);
	BeginQuads();
	{
		Vertex2f(0, 0);
		Vertex2f(0, 9.f);
		Vertex2f(16.f, 9.f);
		Vertex2f(16.f, 0.f);
	}
	GEnd();*/
}

void Renderer::DisableTexture()
{
	glDisable(GL_TEXTURE_2D);
}
