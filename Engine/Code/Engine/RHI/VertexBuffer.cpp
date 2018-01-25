#include "Engine/RHI/VertexBuffer.hpp"

VertexBuffer::VertexBuffer(RHIDevice *rhiDeviceOwner, vertex_t const *vertices, unsigned int const vertex_count, bool isConsole)
{
	// First, describe the buffer
	D3D11_BUFFER_DESC vertexBufferDescription;
	memset(&vertexBufferDescription, 0, sizeof(vertexBufferDescription));

	vertexBufferDescription.ByteWidth = sizeof(vertex_t) * vertex_count;// How much data are we putting into this buffer
	vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER; // What can we bind this data as (in this case, only vertex data)
	vertexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;          
	vertexBufferDescription.StructureByteStride = sizeof(vertex_t);// How large is each element in this buffer

    // Next, setup the initial data 
	D3D11_SUBRESOURCE_DATA initialData;
	memset(&initialData, 0, sizeof(initialData));
	initialData.pSysMem = vertices;

	//create the vertex buffer
	m_dx11Buffer = nullptr;
	rhiDeviceOwner->m_dx11Device->CreateBuffer(&vertexBufferDescription, &initialData, &m_dx11Buffer);

	m_vertexCount = vertex_count;
	m_isConsole = isConsole;
}

VertexBuffer::~VertexBuffer()
{
	DX_SAFE_RELEASE(m_dx11Buffer);
}
