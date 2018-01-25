#pragma once
#if !defined( __RENDER_RASTERSTATE__ )
#define __RENDER_RASTERSTATE__

#include "Engine/DX11/DX11.hpp"
#include "Engine/RHI/rhidevice.hpp"
#include "Engine/Core/StructsAndDefines.hpp"

class RHIDevice;

class RasterState
{
public:
	RasterState(RHIDevice *owner);
	~RasterState();

	inline bool IsValid() const { return (nullptr != m_dx11State); }

public:
	RHIDevice *m_device;
	ID3D11RasterizerState *m_dx11State;
};
#endif 
