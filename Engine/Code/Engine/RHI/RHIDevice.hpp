#pragma once
#if !defined( __RENDER_DEVICE__ )
#define __RENDER_DEVICE__

#include "Engine/DX11/DX11.hpp"
#include "Engine/Core/StructsAndDefines.hpp"
#include "Engine/RHI/VertexBuffer.hpp"
#include "Engine/File/FileUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/DX11/DX11Texture2D.hpp"
#include "Engine/RHI/ShaderProgram.hpp"
#include "Engine/Core/Image.hpp"
#include "Engine/RHI/RHIInstance.hpp"
#include "Engine/RHI/RHIDeviceContext.hpp"
#include <string>

class RHIInstance;
class RHIDeviceContext;
class ShaderProgram;
class ComputeShaderProgram;
class VertexBuffer;
class DX11Texture2D;

class RHIDevice
{
public:
	RHIDevice(RHIInstance *instance, ID3D11Device *dx11Device);
	~RHIDevice();

	ShaderProgram* CreateShaderFromHlslFile(char const *filename);
	ComputeShaderProgram* CreateComputeShaderFromHlslFile(char const *filename);
	ID3DBlob* CompileHLSLToShaderBlob(char const *optionalFileName, void const *sourceCode, size_t const sourceCodeSize, char const *entryPoint, char const* target);

	VertexBuffer* CreateVertexBuffer(vertex_t* vertices, unsigned int vertexCount, bool isConsole);
	DX11Texture2D* createRenderTarget(unsigned int width, unsigned int height);
	DX11Texture2D* createDepthTarget(unsigned int width, unsigned int height);

	RHIDeviceContext* GetImmediateContext();
public:
	RHIInstance *m_instance; // hold a reference so the system doesn't shutdown without me.
	RHIDeviceContext *m_immediateContext; 
	ID3D11Device *m_dx11Device;
	ID3D11ComputeShader * m_dx11ComputerShader;
};
#endif