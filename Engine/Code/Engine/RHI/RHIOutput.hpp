#pragma once
#if !defined( __RENDER_OUTPUT__ )
#define __RENDER_OUTPUT__
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/DX11/DX11Texture2D.hpp"
#include "Engine/DX11/DX11.hpp"

#include "Engine/Core/Win32Includes.hpp"
#include <string>



class RHIDevice;
class Window;
class DX11Texture2D;

class RHIOutput
{
public:
	RHIOutput(RHIDevice *deviceOwner, IDXGISwapChain *swapChain, Window *window);
	~RHIOutput();

	void Present();
	void ClearScreen();

	DX11Texture2D* GetRenderTarget() { return m_renderTarget; };
	bool CreateRenderTarget();

private:
	void DestroyRenderTarget();
	void DestroySwapChain();

public:
	RHIDevice* m_device;
	Window* m_window;
	DX11Texture2D *m_renderTarget;

	IDXGISwapChain *m_SwapChain;

	float clear_color[4];
};
#endif