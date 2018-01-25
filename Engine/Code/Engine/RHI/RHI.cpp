#include "Engine/RHI/RHI.hpp"
#include "Engine/DX11/DX11BlendState.hpp"
RHI* g_theRHI = nullptr;


//--------------------------------Consturctors/Decounstructors-------------------------------------------------------------------------------------------
RHI::RHI()
{
}

RHI::~RHI()
{
	RHIShutdown();
}

void RHI::RHIStartup(HINSTANCE applicationInstanceHandle, WNDCLASSEX windowClassDescription, RHIDevice*& device, RHIDeviceContext*& context, RHIOutput*& output)
{
	RHIInstance *ri = RHIInstance::GetInstance();
	
	m_RHIDevice = new RHIDevice(ri, nullptr);
	m_outputFrame = new RHIOutput(m_RHIDevice, nullptr, nullptr);
	m_deviceContext = new RHIDeviceContext(m_RHIDevice, nullptr);


	ri->CreateOutput(applicationInstanceHandle, windowClassDescription, device, context, output);
	output->CreateRenderTarget();
}

void RHI::RHIShutdown()
{
	D3D11Cleanup();
	delete m_outputFrame;
	delete m_deviceContext;
	delete m_RHIDevice;
}


//------------------------------------Run and Draw Frames------------------------------------------------------------------
void RHI::RunFrame()
{
	m_outputFrame->Present();
}

void RHI::Draw(RHIDeviceContext* deviceContext, ePrimitiveType topology, VertexBuffer* vertexBufferObject, unsigned int const vertexCount)
{
	D3D11_PRIMITIVE_TOPOLOGY dx11PrimitiveTopology;

	switch (topology) 
	{
	case PRIMITIVE_LINES:
		dx11PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	case PRIMITIVE_TRIANGLES:
	default:
		dx11PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	}

	deviceContext->m_dx11Context->IASetPrimitiveTopology(dx11PrimitiveTopology);

	unsigned int stride = sizeof(vertex_t);
	unsigned int offsets = 0;
	deviceContext->m_dx11Context->IASetVertexBuffers(0, 1, &vertexBufferObject->m_dx11Buffer, &stride, &offsets);
	deviceContext->m_dx11Context->Draw(vertexCount, 0);
}



void RHI::ClearScreen()
{
	m_outputFrame->ClearScreen();
}



void RHI::clear_depth(DX11Texture2D *output, float depth /*= 1.0f*/, unsigned char stencil /*= 0*/)
{
	if (output != nullptr) 
		m_deviceContext->m_dx11Context->ClearDepthStencilView(output->m_dxDsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

//------------------------------------Setters----------------------------------------------------------
void RHI::SetRenderTarget(RHIDeviceContext * context, DX11Texture2D * color_target, DX11Texture2D* currentTarget, RHIOutput * output)
{
	if (color_target != nullptr) 
	{
		currentTarget = color_target;
	}
	else 
	{
		currentTarget = output->GetRenderTarget();
	}

	context->m_dx11Context->OMSetRenderTargets(1, &currentTarget->m_dxRtv, nullptr);

}


void RHI::SetViewpoint(RHIDeviceContext* context, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
	// Also, set which region of the screen we're rendering to, in this case, all of it 
	D3D11_VIEWPORT viewport;
	memset(&viewport, 0, sizeof(viewport));
	viewport.TopLeftX = (FLOAT)x;
	viewport.TopLeftY = (FLOAT)y;
	viewport.Width = (FLOAT)w;
	viewport.Height = (FLOAT)h;
	viewport.MinDepth = 0.0f;        // must be between 0 and 1 (defualt is 0);
	viewport.MaxDepth = 1.0f;        // must be between 0 and 1 (default is 1)

	context->m_dx11Context->RSSetViewports(1, &viewport);
}


void RHI::set_color_target(DX11Texture2D *color_target, DX11Texture2D *depth_stencil_target /*= nullptr*/)
{
	m_deviceContext->m_dx11Context->OMSetRenderTargets(1,
		&color_target->m_dxRtv,
		depth_stencil_target != nullptr ? depth_stencil_target->m_dxDsv : nullptr);
}


void RHI::BindUAV(unsigned int tex_index, DX11Texture2D * tex)
{
	m_deviceContext->m_dx11Context->CSSetUnorderedAccessViews(tex_index, 1, &tex->m_dxUav, nullptr);
}

void RHI::UnBindUAV(unsigned int tex_index)
{
	ID3D11UnorderedAccessView* nullUAV = nullptr;
	m_deviceContext->m_dx11Context->CSSetUnorderedAccessViews(tex_index, 1, &nullUAV, nullptr);
}


void RHI::SetTexture2D(unsigned int tex_index, DX11Texture2D* tex)
{
	m_deviceContext->m_dx11Context->VSSetShaderResources(tex_index, 1, &tex->m_dxSrv);
	m_deviceContext->m_dx11Context->PSSetShaderResources(tex_index, 1, &tex->m_dxSrv);

}

void RHI::SetDeviceContext(RHIDeviceContext * context)
{
	m_deviceContext = context;
}

void RHI::SetDevice(RHIDevice* device)
{
	m_RHIDevice = device;
}

void RHI::SetOutput(RHIOutput* output)
{
	m_outputFrame = output;
}

void RHI::SetShader(ShaderProgram * shader)
{
	m_deviceContext->m_dx11Context->VSSetShader(shader->m_dxVertexShader, nullptr, 0U);
	m_deviceContext->m_dx11Context->PSSetShader(shader->m_dxFragmentShader, nullptr, 0U);
	m_deviceContext->m_dx11Context->IASetInputLayout(shader->m_dxInputLayout);
}

void RHI::SetComputeShader(ComputeShaderProgram * shader)
{
	m_deviceContext->m_dx11Context->CSSetShader(shader->m_dxComputeShader, nullptr, 0U);
	//m_deviceContext->m_dx11Context->IASetInputLayout(shader->m_dxInputLayout);
	m_deviceContext->m_dx11Context->Dispatch(64, 64, 1);
}

void RHI::SetSampler(unsigned int samp_index, Sampler * samp)
{
	m_deviceContext->m_dx11Context->VSSetSamplers(samp_index, 1, &samp->m_dxSampler);
	m_deviceContext->m_dx11Context->PSSetSamplers(samp_index, 1, &samp->m_dxSampler);
}

void RHI::SetBlendState(DX11BlendState* bs)
{
	float constant[] = { 1, 1, 1, 1 };
	m_deviceContext->m_dx11Context->OMSetBlendState(bs->dx_state, constant, 0xffffffff);
}
