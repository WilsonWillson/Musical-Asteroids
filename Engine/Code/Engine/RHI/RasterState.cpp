#include "Engine/RHI/RasterState.hpp"


//---------------Construrctors/Decounstructors---------------------------------------------------------
RasterState::RasterState(RHIDevice *owner)
	: m_device(owner), 
	m_dx11State(nullptr)
{
	D3D11_RASTERIZER_DESC rasterDescription;
	MemZero(&rasterDescription);

	rasterDescription.FillMode = D3D11_FILL_SOLID; // D3D11_FILL_WIRE
	rasterDescription.CullMode = D3D11_CULL_BACK;

	// Make it act like OpenGL
	rasterDescription.FrontCounterClockwise = true;

	rasterDescription.AntialiasedLineEnable = false;
	rasterDescription.DepthBias = 0;
	rasterDescription.DepthBiasClamp = 0.0f;
	rasterDescription.SlopeScaledDepthBias = 0.0f;
	rasterDescription.DepthClipEnable = true;
	rasterDescription.ScissorEnable = false;
	rasterDescription.MultisampleEnable = false;

	m_device->m_dx11Device->CreateRasterizerState(&rasterDescription, &m_dx11State);
}

RasterState::~RasterState()
{
	DX_SAFE_RELEASE(m_dx11State);
}