#pragma once
#if !defined( __GAME_SIMPLERENDERER__ )
#define __GAME_SIMPLERENDERER__

#include "Engine/Math/AABB2.hpp"
#include "Engine/RHI/RHI.hpp"
#include "Engine/RHI/RHIInstance.hpp"
#include "Engine/RHI/RasterState.hpp"
#include "Engine/RHI/ConstantBuffer.hpp"
#include "Engine/Math/Matrix4.hpp"
#include "Engine/DX11/DX11BlendState.hpp"
#include "Engine/DX11/DX11TextRenderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/RHI/DepthStencilState.hpp"
#include "Engine/File/ObjReader.hpp"
#include "Engine/Tools/MeshBuilder.hpp"
#include "Engine/Tools/Skeleton.hpp"
#include "Engine/Tools/Pose.hpp"
#include "Engine/Tools/Motion.hpp"
#include "Engine/Tools/SkeletonInstance.hpp"
#include "Engine/RHI/StructuredBuffer.hpp"
#include "Engine/Core/Thread.hpp"
#include "Engine/Core/Win32Includes.hpp"
#include <map>
#include <string>

#define  MAX_LIGHTS 8

class ShaderProgram;
class Sampler;
class VertexBuffer;
class IndexBuffer;
class DX11Texture2D;
class RHIDevice;
class RHIDeviceContext;
class RHIOutput;
class RasterState;
class ConstantBuffer;
class MeshBuilder;

struct matrix_buffer_t
{
	matrix_buffer_t()
		 : eye_position(0.0f, 0.0f, 0.0f, 0.0f)
		 {}

	Matrix4 model;
	Matrix4 view;
	Matrix4 projection;
	Vector4 eye_position;
};

struct clear_constants_buffer_t
{
	clear_constants_buffer_t()
		: CLEAR_COLOR(0.0f, 0.0f, 0.0f, 0.0f)
	{}

	Vector4 CLEAR_COLOR;
	float X_Pan;
	float Y_Pan;
	float zoom_Distance;
	float ITERATIONCOUNT;
	Vector4 PowerCount;
};

struct point_light_buffer
{
	point_light_buffer()
		: light_color(1, 1, 1, 0)
		, light_position(0, 0, 0, 1)
		, attenuation(1, 0, 0, 0)     // will effectively dampen the light - at intensity 1, this will make the light constant
		, spec_attenuation(1, 0, 0, 0)
	{}
	rgba_fl light_color; // <r, g, b, intensity>
	Vector4 light_position; // <x, y, z, padding>  // variables can not cross a 16-byte boundary, so we pad out 
	Vector4 attenuation;    // <constant, linear, quadratic, unused>
	Vector4 spec_attenuation; // <constant, linear, quadratic, unused>
};
struct light_buffer_t
{
	light_buffer_t()
		: ambient(1, 1, 1, 1)
		, spec_factor(0.0f)
		, spec_power(0.0f)
	{}
	rgba_fl ambient;  // <r, g, b, intensity>
	point_light_buffer lightBufferArray[MAX_LIGHTS];
	// SURFACE INFORMATION
	float spec_factor;
	float spec_power;
	Vector2 unused;
};

struct blend_state_t
{
	bool enabled;
	eBlendFactor src_factor;
	eBlendFactor dst_factor;
};

class SimpleRenderer;
struct load_texture_job_t
{
	char const * filename;
	Image *image;
	DX11Texture2D *texture; // texture to load into
	SimpleRenderer* renderer;
};

typedef void(*on_texture_loaded_cb)(DX11Texture2D *tex, void *user_arg);


struct load_text_job_t
{
	Vector2 pos;
	int scale;
	Rgba color;
	DX11TextRenderer text;
	std::string textToWrite;
	//char*  textToWrite;
	SimpleRenderer* renderer;
};

class SimpleRenderer
{
public:
	//Setting Up Renderer
	SimpleRenderer();
	~SimpleRenderer();
	void Setup(HINSTANCE applicationInstanceHandle, WNDCLASSEX windowClassDescription);

	//Drawing
	void Present();
	void Draw(ePrimitiveType topology, VertexBuffer *vertexBufferObject, unsigned int  const vertexCount);
	void DrawVertexBufferArray(ePrimitiveType topology, std::vector<VertexBuffer *> m_vertexBufferArray);
	void DrawVertexBufferArray(ePrimitiveType topology, std::vector<VertexBuffer *> m_vertexBufferArray, int bufferIndex);
	void ClearScreen();
	void ClearDepth(float depth = 1.0f, unsigned char stencil = 0);

	//Creation
	VertexBuffer* CreateQuad(float x, float y, float z, float vec2XMin, float vec2YMin, float vec2XMax, float vec2YMax, bool isConsole);

	void enable_blend(eBlendFactor src, eBlendFactor dest);
	void disable_blend();

	//Draw Calls
	void draw_quad2d(Vector2 bottom_left, Vector2 top_right, Vector2 uvBottomLeft, Vector2 uvBottomRight, Rgba color);
	void draw_quad2d(AABB2 bounds, AABB2 uvbounds, rgba_fl color);
	void draw_quad3d(Vector3 const &origin, Vector3 const &right, float neg_x_extents, float pos_x_extents, Vector3 const &up, float neg_y_extents, float pos_y_extents, Rgba color);
	void draw_line(Vector3 startPoint, Vector3 endPoint, rgba_fl color);
	void draw_point(Vector3 const &p, rgba_fl const &color /*= rgba_fl::WHITE*/);
	void DrawDX11Text2D(Vector2 pos, float scale, Rgba color, DX11TextRenderer text, char* const textToWrite);

	void DrawCube(class Vector3, Rgba color);
	void DrawBasis();
	void drawPlane(Vector3 cords, Rgba color);
	void drawTwoSidedPlane(Vector3 pos, Rgba color);



	//Setters
	//void SetRenderTarget(DX11Texture2D *colorTarget);
	void SetRenderTarget(DX11Texture2D *color_target, DX11Texture2D *depth_target = nullptr);
	void SetViewport(unsigned int  x, unsigned int  y, unsigned int  width, unsigned int  height);

	void set_depth_stencil_state(DepthStencilState *dss);
	void SetTexture2d(unsigned int texIndex, DX11Texture2D *tex);
	inline void SetTexture2d(DX11Texture2D *texture) { SetTexture2d(0, texture); }
	inline void SetUAVTexture2d(DX11Texture2D *texture) { SetUAVTexture2d(0, texture); }
	inline void UnBindUAVTexture2d() { UnBindUAVTexture2d(0); }
	void UnBindUAVTexture2d(unsigned int texIndex);
	void SetUAVTexture2d(unsigned int texIndex, DX11Texture2D *tex);
	void SetShader(ShaderProgram *shader);

	//void SetComputeShader(ShaderProgram *shader);
	void SetComputeShader(ComputeShaderProgram *shader);
	void UpdateClearColorBuffer();
	void SetSampler(unsigned int  samplerIndex, Sampler *sampler);
	inline void SetSampler(Sampler *sampler) { SetSampler(0, sampler); }

	void SetConstantBuffer(unsigned int index, ConstantBuffer * timeConstants);

	inline void set_diffuse(DX11Texture2D *texture) { SetTexture2d(0, (texture == nullptr) ? white_texture : texture); }
	inline void set_normal(DX11Texture2D *texture) { SetTexture2d(1, (texture == nullptr) ? flat_normal_texture : texture); }
	inline void set_spec(DX11Texture2D *texture) { SetTexture2d(2, (texture == nullptr) ? white_texture : texture); }
	void set_ambient_light(float intensity, rgba_fl const &color /*= rgba_fl::WHITE*/);
	void set_point_light(int index, Vector3 const &pos, rgba_fl const &color, float intensity /*= 1.0f*/, Vector3 const &attenuation /*= vec3( 0, 0, 1 )*/, Vector3 const &spec_attenuation = Vector3(0, 0, 1));
	void set_projection_matrix(Matrix4 const &proj);
	void set_model_matrix(Matrix4 const &model);
	void set_view_matrix(Matrix4 const &proj);
	void set_camera_matrix(Matrix4 const &camera);
	void set_eye_position(Vector4 const &eye_position);
	void set_specular_constants(float const spec_power, float const spec_factor = 1.0f);
	void set_ortho_projection(Vector2 const &bottom_left, Vector2 const &top_right);
	void set_perspective_projection(float const fov_radians, float aspect_ratio, float const nz, float const fz);
	void set_model_projection(float degrees, Vector4 translation);

	void UpdateLighting();

	MeshBuilder* CreateOrGetMesh(const char* meshFilePath, Skeleton* skeleton, bool readFromBinaryFile);
	MeshBuilder* GetMesh(const char* imageFilePath);

	Skeleton* CreateOrGetSkeleton(const char* meshFilePath, bool readFromBinaryFile);
	Skeleton* GetSkeleton(const char* imageFilePath);

	Motion* CreateOrGetMotion(const char* meshFilePath, Skeleton* skeleton, bool readFromBinaryFile);
	Motion* GetMotion(const char* imageFilePath);

	ComputeShaderProgram* CreateorGetComputeShaderProgram(const char* shaderFilePath);
	ComputeShaderProgram* GetComputeShaderProgram(const char* shaderFilePath);
	ShaderProgram* CreateorGetShaderProgram(const char* shaderFilePath);
	ShaderProgram* GetShaderProgram(const char* shaderFilePath);

	DX11Texture2D* CreateOrGetDX11Texture2D(const char* dx11FilePath);
	DX11Texture2D* GetDX11Texture2D(const char* dx11FilePath);
	DX11Texture2D* LoadTextureAsync(char const *filename, on_texture_loaded_cb cb = nullptr, void *user_arg = nullptr);




public:
	RHIDeviceContext *m_context;
	RHIDevice *m_device;
	RHIOutput *m_output;
	DX11Texture2D* m_currentTarget;
	RasterState *m_defaultRasterState;
	//std::vector<VertexBuffer*> vertexBufferArray;

	matrix_buffer_t matrix_data;
	ConstantBuffer *matrix_cb;

	clear_constants_buffer_t clear_constants_data;
	ConstantBuffer* clear_cb;

	light_buffer_t	light_data;
	ConstantBuffer*	light_cb;
	//light_buffer_t lightBufferArray[MAX_LIGHTS];
	ShaderProgram*	m_LightShader;

	blend_state_t blend_state;
	DX11BlendState* current_blend_state;
	ShaderProgram*	m_colorShader;

	DX11Texture2D *default_depth_stencil;
	DX11Texture2D *current_depth_stencil;
	DX11Texture2D *white_texture;
	DX11Texture2D *flat_normal_texture;

	DX11Texture2D * Texture2d;
	DX11Texture2D* diffuseTexture;

	depthStencilDesc_t depth_stencil_desc;
	DepthStencilState *depth_stencil_state;

	StructuredBuffer* m_StructreBuffer;

	ThreadSafeQueue<load_text_job_t>* g_RenderTextItems;

	
public:
	void setViewProjection(Vector3 m_pos, float rollAroundX, float pitchAroundY, float yawAroundZ);
	void setBillboardProjection(Vector3 m_pos, float pitchAroundX, float yawAroundY, float rollAroundZ);
	void enable_depth(bool test, bool write);
	void DrawOBJObject(ObjReader* objObject);
	void DrawOBJObjectFlipBasis(ObjReader* objObject);
	void DrawMesh(MeshBuilder *mesh);
	void DrawSkeleton(Skeleton* skeleton);
	void DrawPose(Motion* currentMotion, Skeleton* skeleton);
	void SetStructureBuffer(const void* buffer, unsigned int  structureSize, unsigned int structureCounter);
	void UpdateStructureBuffer(const void* buffer);

private:
		std::map< std::string, MeshBuilder* >		m_alreadyLoadedMeshes;
		std::map<std::string, Skeleton* >		m_alreadyLoadedSkeletons;
		std::map<std::string, Motion* >		m_alreadyLoadedMotions;

		std::map<std::string, ShaderProgram* >		m_alreadyLoadedShaders;
		std::map<std::string, ComputeShaderProgram* >		m_alreadyLoadedComputeShaders;
		std::map<std::string, DX11Texture2D* >		m_alreadyLoadedTextures;

public:
	void setComputeConstantBuffer();
};
#endif