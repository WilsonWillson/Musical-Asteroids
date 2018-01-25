#pragma once
#if !defined( __RHI_SHADERPROGRAM__ )
#define __RHI_SHADERPROGRAM__

#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/DX11/DX11.hpp"

enum eShaderStage
{
	SHADER_VERTEX,    // maps to target "vs_5_0", or Vertex Shader Model 5 (currently latest)
	SHADER_FRAGMENT,  // maps to target "ps_5_0", or Pixel Shader Model 5 (currently latest)
};

class ShaderProgram;
class ComputeShaderProgram 
{
public:
	ComputeShaderProgram(ID3D11ComputeShader* computeShader, ID3DBlob * computeShaderByteCode);
	~ComputeShaderProgram();

public:
	ID3D11ComputeShader* m_dxComputeShader;
	ID3DBlob *m_csByteCode;
};






class ShaderProgram
{
public:
	ShaderProgram(RHIDevice *device, ID3D11VertexShader *vertexShader, ID3D11PixelShader *fragmentShader, ID3DBlob *vertexShaderByteCode, ID3DBlob * fragementShaderByteCode);
	ShaderProgram(ID3D11ComputeShader* computeShader, ID3DBlob * computeShaderByteCode);
	~ShaderProgram();

	inline bool IsValid() const { return (m_dxVertexShader != nullptr) && (m_dxFragmentShader != nullptr); }

private:
	void CreateInputLayout(RHIDevice* device);

public:
	ID3D11VertexShader *m_dxVertexShader;
	ID3D11PixelShader *m_dxFragmentShader;
	ID3D11ComputeShader* m_dxComputeShader;
	ID3D11InputLayout *m_dxInputLayout;
	ID3DBlob *m_vsByteCode;
	ID3DBlob *m_fsByteCode;
	ID3DBlob *m_csByteCode;
};
#endif