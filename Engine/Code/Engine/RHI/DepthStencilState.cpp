#include "Engine/RHI/DepthStencilState.hpp"



//------------------------------------------------------------------------
DepthStencilState::DepthStencilState(RHIDevice *rhiDeviceOwner, depthStencilDesc_t const &depthStencilDescriptionStruct)
	: m_device(rhiDeviceOwner), 
	m_dx11DepthStencilState(nullptr)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDescription;
	MemZero(&depthStencilDescription);

	depthStencilDescription.DepthEnable = depthStencilDescriptionStruct.depth_writing_enabled || depthStencilDescriptionStruct.depth_test_enabled;
	depthStencilDescription.DepthWriteMask = depthStencilDescriptionStruct.depth_writing_enabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDescription.DepthFunc = depthStencilDescriptionStruct.depth_test_enabled ? D3D11_COMPARISON_LESS : D3D11_COMPARISON_ALWAYS;
	depthStencilDescription.StencilEnable = FALSE;

	HRESULT result = m_device->m_dx11Device->CreateDepthStencilState(&depthStencilDescription, &m_dx11DepthStencilState);
	UNREFERENCED(result); // ASSERT
}

//------------------------------------------------------------------------
DepthStencilState::~DepthStencilState()
{
	DX_SAFE_RELEASE(m_dx11DepthStencilState);
}