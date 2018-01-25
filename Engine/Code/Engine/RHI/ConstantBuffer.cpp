#include "Engine/RHI/ConstantBuffer.hpp"


//-------------------------------Constructors / Decounstructors---------------------------
ConstantBuffer::ConstantBuffer(RHIDevice *device, void const *buffer, size_t const size)
{
	m_bufferSize = size;

	// First, describe the buffer
	D3D11_BUFFER_DESC bufferDescription;
	memset(&bufferDescription, 0, sizeof(bufferDescription));

	bufferDescription.ByteWidth = (unsigned int)m_bufferSize; // How much data are we putting into this buffer
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // What can we bind this data as (in this case, only vertex data)
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;  // Hint on how this memory is used (in this case, it is immutable, or constant - can't be changed)
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDescription.StructureByteStride = 0;  // How large is each element in this buffer

    // Next, setup the initial data
	D3D11_SUBRESOURCE_DATA initialData;
	memset(&initialData, 0, sizeof(initialData));
	initialData.pSysMem = buffer;

	//create the vertex buffer
	m_dxBuffer = nullptr;
	device->m_dx11Device->CreateBuffer(&bufferDescription, &initialData, &m_dxBuffer);
}

ConstantBuffer::~ConstantBuffer()
{
	DX_SAFE_RELEASE(m_dxBuffer);
}

//---------------------Update---------------------------------------------------
bool ConstantBuffer::Update(RHIDeviceContext *rhiContext, void const *buffer)
{
	D3D11_MAPPED_SUBRESOURCE d3d11Resource;

	ID3D11DeviceContext *dx11Context = rhiContext->m_dx11Context;
	if (SUCCEEDED(dx11Context->Map(m_dxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0U, &d3d11Resource)))
	{
		memcpy(d3d11Resource.pData, buffer, m_bufferSize);
		dx11Context->Unmap(m_dxBuffer, 0);
		return true;
	}

	return false;
}

