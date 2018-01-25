#include "Engine/DX11/DX11BlendState.hpp"

static D3D11_BLEND  ToDXFactor(eBlendFactor const factor)
{
	switch (factor) 
	{
	case BLEND_ONE:
		return D3D11_BLEND_ONE;
	case BLEND_ZERO:
		return D3D11_BLEND_ZERO;
	case BLEND_SRC_ALPHA:
		return D3D11_BLEND_SRC_ALPHA;
	case BLEND_INV_SRC_ALPHA:
		return D3D11_BLEND_INV_SRC_ALPHA;
	default:
		return D3D11_BLEND_ONE;
	};
}


DX11BlendState::DX11BlendState(RHIDevice* owner, bool enabled, eBlendFactor const src_factor, eBlendFactor dest_factor)
	:device(owner),
	dx_state(nullptr)
{
	D3D11_BLEND_DESC belndDescription;
	MemZero(&belndDescription);
	belndDescription.AlphaToCoverageEnable = false;
	belndDescription.IndependentBlendEnable = false;

	//Color Blending
	belndDescription.RenderTarget[0].BlendEnable = enabled;
	belndDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	belndDescription.RenderTarget[0].SrcBlend = ToDXFactor(src_factor);
	belndDescription.RenderTarget[0].DestBlend = ToDXFactor(dest_factor); 

	//Alpha Blending
	belndDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	belndDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	belndDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	belndDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	//Create It
	ID3D11Device *dx_device = device->m_dx11Device;
	HRESULT hr = dx_device->CreateBlendState(&belndDescription, &dx_state);

	SUCCEEDED(hr);

}

DX11BlendState::~DX11BlendState()
{
	DX_SAFE_RELEASE(dx_state);
}
