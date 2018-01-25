
#include "Engine/RHI/sampler.hpp"


//-----------------------Constructor/Decounstror-------------------------------------------------
Sampler::Sampler(RHIDevice *deviceOwner)
	: m_device(deviceOwner), 
	  m_dxSampler(nullptr)
{}

Sampler::Sampler(RHIDevice *deviceOwner, eFilterMode minFilter, eFilterMode magFilter)
	: Sampler(deviceOwner)
{
	CreateSampler(minFilter, magFilter);
}


Sampler::~Sampler()
{
	DX_SAFE_RELEASE(m_dxSampler);
}

bool Sampler::CreateSampler(eFilterMode minFilter, eFilterMode magFilter)
{
	D3D11_SAMPLER_DESC description;
	MemZero(&description);

	description.Filter = GetDXFilterMode(minFilter, magFilter);
	description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	description.MinLOD = 0;
	description.MaxLOD = 0;

	ID3D11Device *dx11Device = m_device->m_dx11Device;
	HRESULT hResult = dx11Device->CreateSamplerState(&description, &m_dxSampler);

	return SUCCEEDED(hResult);
}

//-------------------------Getter-----------------------------------------------
 D3D11_FILTER Sampler::GetDXFilterMode(eFilterMode minFilter, eFilterMode magFilter)
{
	if (minFilter == FILTER_POINT) 
	{
		if (magFilter == FILTER_POINT)
			return D3D11_FILTER_MIN_MAG_MIP_POINT;
		else// LINEAER
			return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
	}
	// assume linear
	else 
	{
		if (magFilter == FILTER_POINT)
			return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
		
		else //assume linear
			return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	}
}