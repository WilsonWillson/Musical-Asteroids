#pragma once
#include "Engine/RHI/RHIDevice.hpp"
#include <cstdint>
#include "stdint.h"
#include <d3d11.h>
#include <d3d10.h>
#include <DXGI.h>

class RHIDevice;


class Texture2D
{
public:
	Texture2D(RHIDevice *device, ID3D11Texture2D *tex);
	~Texture2D();

	unsigned int get_width();
	unsigned int get_height();

	void set_width(unsigned int targetWidth);
	void set_height(unsigned int targetHeight);

	inline bool is_valid() const { return (dx_resource != nullptr); }
	inline bool is_render_target() const { return (dx_rtv != nullptr); }

public:
	RHIDevice *device;

	ID3D11Texture2D *dx_resource;
	ID3D11RenderTargetView *dx_rtv;

	unsigned int width;
	unsigned int height;
};
