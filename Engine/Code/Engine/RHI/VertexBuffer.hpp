#pragma once
#if !defined( __RHI_VERTEX_BUFFER__ )
#define __RHI_VERTEX_BUFFER__

#include "Engine/Core/StructsAndDefines.hpp"
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/DX11/DX11.hpp"

class RHIDevice;


class VertexBuffer
{
public:
	VertexBuffer(RHIDevice *deviceOwner,vertex_t const *vertices, unsigned int const vertex_count, bool console);
	~VertexBuffer();

	inline bool IsValid() const { return (m_dx11Buffer != nullptr); }

public:
	ID3D11Buffer *m_dx11Buffer;
	int m_vertexCount;
	bool m_isConsole;
};
#endif