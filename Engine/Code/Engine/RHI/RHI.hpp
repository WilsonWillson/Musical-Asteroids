#pragma once
#if !defined( __RHI__ )
#define __RHI__ 

#include "Engine/DX11/DX11.hpp"
#include "Engine/DX11/DX11Texture2D.hpp"
#include "Engine/RHI/RHIInstance.hpp"
#include "Engine/RHI/RHIDeviceContext.hpp"
#include "Engine/RHI/Sampler.hpp"
#include "Engine/RHI/ShaderProgram.hpp"
#include "Engine/RHI/VertexBuffer.hpp"


#include "Engine/Core/Win32Includes.hpp"
#include <string>

// +1 for immediate/$Global constant buffer, so actually 16
#define MAX_CONSTANT_BUFFERS (15)   

// 4096 16-byte elements.
#define MAX_CONSTANT_BUFFER_SIZE    (16 * 4096)    

#define MAX_BOUND_COLOR_TARGETS (8)

// Determines how a resource is meant to be used.
enum eBufferUsage : unsigned int
{
	BUFFERUSAGE_GPU,     // GPU Read/Write, CPU Full Writes        (RenderTargets?)
	BUFFERUSAGE_STATIC,  // GPU Read, Written only at creation     (TEXTURES, STATIC MESHES)
	BUFFERUSAGE_DYNAMIC, // GPU Read, CPU Write (Partial or Full)  (DYNAMIC MESHES/DYNAMIC TEXTURES)
	BUFFERUSAGE_STAGING, // GPU Read/Write, CPU Read/Write         Slowest - image composition/manipulation
};

//------------------------------------------------------------------------
enum ePrimitiveType : unsigned int
{
	PRIMITIVE_NONE,
	PRIMITIVE_POINTS,
	PRIMITIVE_LINES,
	PRIMITIVE_TRIANGLES,
};


class RHIOutput;
class RHIDeviceContext;
class RHIDevice;
class Sampler;
class DX11BlendState;

class RHI
{
public:
	RHI();
	~RHI();

	void RunFrame();
	void RHIStartup(HINSTANCE applicationInstanceHandle, WNDCLASSEX windowClassDescription, RHIDevice*& device, RHIDeviceContext*& context, RHIOutput*& output);
	void RHIShutdown();
	void ClearScreen();
	void clear_depth(DX11Texture2D *output, float depth = 1.0f, unsigned char stencil = 0);


	void Draw(RHIDeviceContext* context, ePrimitiveType topology, VertexBuffer* vertexBufferObject, unsigned int const vertexCount);

	void SetTexture2D(unsigned int texIndex, DX11Texture2D *tex);
	void SetDeviceContext(RHIDeviceContext * context);
	void SetDevice(RHIDevice* device);
	void SetOutput(RHIOutput* output);
	void SetShader(ShaderProgram * shader);
	//void SetComputeShader(ShaderProgram * shader);
	void SetComputeShader(ComputeShaderProgram * shader);
	void SetBlendState(DX11BlendState* bs);
	void SetSampler(unsigned int samp_index, Sampler * samp);
	void SetRenderTarget(RHIDeviceContext* context, DX11Texture2D * colorTarget, DX11Texture2D* currentTarget, RHIOutput * output);
	void SetViewpoint(RHIDeviceContext* context, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
	void set_color_target(DX11Texture2D *color_target, DX11Texture2D *depth_stencil_target = nullptr);

	void BindUAV(unsigned int tex_index, DX11Texture2D * tex);
	void UnBindUAV(unsigned int tex_index);
public:
	RHIDevice *m_RHIDevice;
	RHIOutput* m_outputFrame;
	RHIDeviceContext* m_deviceContext;
	
};
#endif
extern RHI* g_theRHI;
