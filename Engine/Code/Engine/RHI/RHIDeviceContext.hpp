#pragma once
#if !defined( __RENDER_CONTEXT__ )
#define __RENDER_CONTEXT__

#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/RHI/RasterState.hpp"
#include "Engine/DX11/DX11.hpp"

class RHIDevice; 
class DX11Texture2D;
class RasterState;
class StructuredBuffer;

class RHIDeviceContext
{
public:
	RHIDeviceContext(RHIDevice* owner, ID3D11DeviceContext* dx11Context);
	~RHIDeviceContext();

	void SetRasterState(RasterState* rasterState);

	void set_structured_buffer(unsigned int bind_point, StructuredBuffer *buffer);
public:
	RHIDevice* m_RHIDevice;
	ID3D11DeviceContext* m_dx11Context;
};
#endif