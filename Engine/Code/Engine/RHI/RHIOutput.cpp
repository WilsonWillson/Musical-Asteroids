#include "Engine/RHI/RHIOutput.hpp"
#include "Engine/Core/Windows.hpp"

//-----------------------------Creation/Desctrution---------------------------------------
RHIOutput::RHIOutput(RHIDevice *owner, IDXGISwapChain *chain, Window *wnd)
{
	m_SwapChain = chain;
	m_device = owner;
	m_window = wnd;

	clear_color[0] = .000f;
	clear_color[1] = 0.000f;
	clear_color[2] = 0.000f;
	clear_color[3] = 1.0f;
}

bool RHIOutput::CreateRenderTarget()
{
	ID3D11Texture2D *back_buffer = nullptr;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);
	m_renderTarget = new DX11Texture2D(m_device, back_buffer);
	DX_SAFE_RELEASE(back_buffer);
	return true;
}


RHIOutput::~RHIOutput()
{
	//RHIInstance::ReportLiveObject();
	DestroySwapChain();
	//RHIInstance::ReportLiveObject();
	// DestroyRenderTarget();
}

void RHIOutput::DestroyRenderTarget()
{
	if (nullptr != m_renderTarget) 
	{
		delete m_renderTarget;
		m_renderTarget = nullptr;
	}
}

void RHIOutput::DestroySwapChain()
{
	DX_SAFE_RELEASE(m_SwapChain);
}

//---------------------------Update Window-------------------------------------------

void RHIOutput::Present()
{
	pump();
}

void RHIOutput::ClearScreen()
{
	cleanScreen(clear_color);
}