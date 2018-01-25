#pragma once

#include "Engine/DX11/DX11.hpp"
#include "Engine/DX11/DX11Texture2D.hpp"
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/RHI/RHIDeviceContext.hpp"

class StructuredBuffer
{
public:
	// Note, it takes an object size, and object_count instead
	// of the arryas total size, as object_size matters in 
	// creation!
	StructuredBuffer(RHIDevice *owner, void const *initial_data, unsigned int object_size, unsigned int object_count);
	~StructuredBuffer();

	// New for a structured buffer!
	bool create_views(RHIDevice* device);

	bool update(RHIDeviceContext *context, void const *buffer);

	inline bool is_valid() const { return (dx_buffer != nullptr); }

public:
	ID3D11Buffer *dx_buffer;
	ID3D11ShaderResourceView *dx_srv;

	unsigned int obj_count;
	unsigned int obj_size;
	size_t buffer_size;
};