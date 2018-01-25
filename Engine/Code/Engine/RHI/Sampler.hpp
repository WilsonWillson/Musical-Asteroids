#pragma once
#if !defined( __RENDER_SAMPLER__ )
#define __RENDER_SAMPLER__


#include "Engine/DX11/DX11.hpp"
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/Core/StructsAndDefines.hpp"

enum eFilterMode
{
	FILTER_POINT,
	FILTER_LINEAR,
};

class RHIDevice;

class Sampler
{
public:
	Sampler(RHIDevice *deviceOwner);
	Sampler(RHIDevice *deviceOwner, eFilterMode minFilter, eFilterMode magFilter);
	~Sampler();

	inline bool is_valid() const { return (nullptr != m_dxSampler); }

private:
	bool CreateSampler(eFilterMode minFilter, eFilterMode magFilter);
	static D3D11_FILTER GetDXFilterMode(eFilterMode minFilter, eFilterMode magFilter);

public:
	RHIDevice *m_device;
	ID3D11SamplerState *m_dxSampler;
};
#endif 
