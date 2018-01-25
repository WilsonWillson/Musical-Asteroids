#include "Engine/RHI/RHIDeviceContext.hpp"
#include "Engine/RHI/StructuredBuffer.hpp"
#include "Engine/DX11/DX11BlendState.hpp"


//----------------------------------------Constructor/Deconstructor----------------------------
RHIDeviceContext::RHIDeviceContext(RHIDevice* owner, ID3D11DeviceContext* ctx)
	: m_dx11Context(ctx)
{
	m_RHIDevice = owner;
}

RHIDeviceContext::~RHIDeviceContext()
{
	DX_SAFE_RELEASE(m_dx11Context);
}

//---------------------------------Setters-----------------------------------
void RHIDeviceContext::SetRasterState(RasterState *rasterState)
{
	m_dx11Context->RSSetState(rasterState->m_dx11State);
}


//------------------------------------------------------------------------
// BINDING!
//------------------------------------------------------------------------
void RHIDeviceContext::set_structured_buffer(unsigned int bind_point, StructuredBuffer *buffer)
{
	// todo: check for null, and unbind if so.
	m_dx11Context->VSSetShaderResources(bind_point, 1, &buffer->dx_srv);
	m_dx11Context->PSSetShaderResources(bind_point, 1, &buffer->dx_srv);
}