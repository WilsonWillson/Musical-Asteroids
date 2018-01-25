#pragma once
#if !defined( __RENDER_DEPTHSTENCILSTATE__ )
#define __RENDER_DEPTHSTENCILSTATE__

#include "Engine/DX11/DX11.hpp"
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/Core/StructsAndDefines.hpp"

struct depthStencilDesc_t
{
	bool depth_writing_enabled; // if writing a pixel - also write a depth
	bool depth_test_enabled;    // only write a pixel if depth written is less than or equal current current depth
};


class RHIDevice;
class DepthStencilState
{
public:
	DepthStencilState(RHIDevice *owner, depthStencilDesc_t const &depthStencilDescription);
	~DepthStencilState();

	inline bool is_valid() const { return (nullptr != m_dx11DepthStencilState); }

public:
	RHIDevice* m_device;
	ID3D11DepthStencilState* m_dx11DepthStencilState;
};


#endif 
