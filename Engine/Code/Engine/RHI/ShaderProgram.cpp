#include "Engine/RHI/ShaderProgram.hpp"


ShaderProgram::ShaderProgram(RHIDevice *device, ID3D11VertexShader *vertexShader, ID3D11PixelShader *fragmentShader, ID3DBlob *vertexShaderByteCode, ID3DBlob * fragementShaderByteCode)
	:m_dxVertexShader(vertexShader),
	m_dxFragmentShader(fragmentShader),
	m_dxComputeShader(nullptr),
	m_vsByteCode(vertexShaderByteCode),
	m_fsByteCode(fragementShaderByteCode),
	m_csByteCode(nullptr)
{
	m_dxInputLayout = nullptr;
	CreateInputLayout(device);
}

ShaderProgram::ShaderProgram(ID3D11ComputeShader* computeShader, ID3DBlob * computeShaderByteCode)
	:m_dxVertexShader(nullptr),
	m_dxFragmentShader(nullptr),
	m_dxComputeShader(computeShader),
	m_vsByteCode(nullptr),
	m_fsByteCode(nullptr),
	m_csByteCode(computeShaderByteCode)
{
	m_dxInputLayout = nullptr;
}


ShaderProgram::~ShaderProgram()
{
	DX_SAFE_RELEASE(m_dxVertexShader);
	DX_SAFE_RELEASE(m_dxFragmentShader);
	DX_SAFE_RELEASE(m_dxInputLayout);
	DX_SAFE_RELEASE(m_vsByteCode);
	DX_SAFE_RELEASE(m_fsByteCode);
}


// Finally, we need to create an input layout - so that we know how to get our vertices to the shader - this requires still having the bytecode for your fragment shader around.
void ShaderProgram::CreateInputLayout(RHIDevice* rhiDevice)
{
	
	D3D11_INPUT_ELEMENT_DESC input_description[7];
	memset(input_description, 0, sizeof(input_description));

	// Map Position
	input_description[0].SemanticName = "POSITION";// name we gave this input
	input_description[0].SemanticIndex = 0;// Semantics that share a name (or are large) are spread over multiple indices (matrix4x4s are four floats for instance)
	input_description[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;// Type this data is (float3/vec3 - so 3 floats)
	input_description[0].InputSlot = 0U;// Input Pipe this comes from
	input_description[0].AlignedByteOffset = offsetof(vertex_t, position);// Offset into this pipe to get to the data we want (this resolves to 0)
	input_description[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;// What is this data for (per vertex data is the only one we'll be dealing with in this class)
	input_description[0].InstanceDataStepRate = 0U;// If this were instance data - how often do we step it (0 for vertex data)

	//Normals
	input_description[1].SemanticName = "NORMAL";
	input_description[1].SemanticIndex = 0;
	input_description[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	input_description[1].InputSlot = 0U;
	input_description[1].AlignedByteOffset = offsetof(vertex_t, normal);
	input_description[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	input_description[1].InstanceDataStepRate = 0U;

	//UV Cords of Text
	input_description[2].SemanticName = "UVCORDS";
	input_description[2].SemanticIndex = 0;
	input_description[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	input_description[2].InputSlot = 0U; 
	input_description[2].AlignedByteOffset = offsetof(vertex_t, uv);
	input_description[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	input_description[2].InstanceDataStepRate = 0U;

	//Tint of the Texture
	input_description[3].SemanticName = "TINT";
	input_description[3].SemanticIndex = 0;
	input_description[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	input_description[3].InputSlot = 0U;
	input_description[3].AlignedByteOffset = offsetof(vertex_t, color);
	input_description[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	input_description[3].InstanceDataStepRate = 0U;

	//Normals
	input_description[4].SemanticName = "TANGENT";
	input_description[4].SemanticIndex = 0;
	input_description[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	input_description[4].InputSlot = 0U;
	input_description[4].AlignedByteOffset = offsetof(vertex_t, tangent);
	input_description[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	input_description[4].InstanceDataStepRate = 0U;

	input_description[5].SemanticName = "BONEINDICES";
	input_description[5].SemanticIndex = 0;
	input_description[5].Format = DXGI_FORMAT_R32G32B32A32_UINT;
	input_description[5].InputSlot = 0U;
	input_description[5].AlignedByteOffset = offsetof(vertex_t, boneIndices);
	input_description[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	input_description[5].InstanceDataStepRate = 0U;

	input_description[6].SemanticName = "BONEWEIGHT";
	input_description[6].SemanticIndex = 0;
	input_description[6].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	input_description[6].InputSlot = 0U;
	input_description[6].AlignedByteOffset = offsetof(vertex_t, boneWeights);
	input_description[6].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	input_description[6].InstanceDataStepRate = 0U;

	rhiDevice->m_dx11Device->CreateInputLayout(input_description, ARRAYSIZE(input_description), m_vsByteCode->GetBufferPointer(), m_vsByteCode->GetBufferSize(), &m_dxInputLayout);

}

ComputeShaderProgram::ComputeShaderProgram(ID3D11ComputeShader* computeShader, ID3DBlob * computeShaderByteCode)
	:m_dxComputeShader(computeShader),
	m_csByteCode(computeShaderByteCode)
{
}

ComputeShaderProgram::~ComputeShaderProgram()
{
	DX_SAFE_RELEASE(m_dxComputeShader);
	DX_SAFE_RELEASE(m_csByteCode);
}
