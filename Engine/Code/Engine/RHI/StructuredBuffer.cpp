#include "Engine/RHI/StructuredBuffer.hpp"

//------------------------------------------------------------------------
// IMPLEMENTATION
//------------------------------------------------------------------------
StructuredBuffer::StructuredBuffer(RHIDevice *device, void const *buffer, unsigned int object_size,unsigned int object_count)
{
	dx_srv = nullptr;
	buffer_size = object_size * object_count;
	obj_count = object_count;
	obj_size = object_size;

	// First, describe the buffer
	D3D11_BUFFER_DESC vb_desc;
	memset(&vb_desc, 0, sizeof(vb_desc));

	vb_desc.ByteWidth = (unsigned int)buffer_size;                // How much data are we putting into this buffer
	vb_desc.Usage = D3D11_USAGE_DYNAMIC;                  // Hint on how this memory is used (in this case, it is immutable, or constant - can't be changed)
														  // for limitations/strenghts of each, see;  
														  //    https://msdn.microsoft.com/en-us/library/windows/desktop/ff476259(v=vs.85).aspx
	vb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// CHANGES FROM CONSTANT BUFFER!!!
	vb_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;       // What can we bind this data as (in this case, only vertex data)
	vb_desc.StructureByteStride = object_size;            // How large is each element in this buffer
	vb_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	// Next, setup the initial data (required since this is an immutable buffer - so it must be instantiated at creation time)
	D3D11_SUBRESOURCE_DATA initialData;
	memset(&initialData, 0, sizeof(initialData));
	initialData.pSysMem = buffer;

	// Finally create the vertex buffer
	dx_buffer = nullptr;
	device->m_dx11Device->CreateBuffer(&vb_desc, &initialData, &dx_buffer);
	create_views(device);
}

//------------------------------------------------------------------------
StructuredBuffer::~StructuredBuffer()
{
	DX_SAFE_RELEASE(dx_buffer);
	DX_SAFE_RELEASE(dx_srv);
}

//------------------------------------------------------------------------
bool StructuredBuffer::create_views(RHIDevice* device)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	MemZero(&desc);

	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	desc.Buffer.ElementOffset = 0U;
	desc.Buffer.NumElements = obj_count;

	HRESULT result = device->m_dx11Device->CreateShaderResourceView(dx_buffer, &desc, &dx_srv); result;

	return true;
}

//------------------------------------------------------------------------
bool StructuredBuffer::update(RHIDeviceContext *context, void const *buffer)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	ID3D11DeviceContext *dx_context = context->m_dx11Context;
	if (SUCCEEDED(dx_context->Map(dx_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0U, &resource))) {
		memcpy(resource.pData, buffer, buffer_size);
		dx_context->Unmap(dx_buffer, 0);

		return true;
	}

	// ASSERT?
	return false;
}