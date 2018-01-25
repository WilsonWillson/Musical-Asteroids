#pragma once
#if !defined( __RENDER_DX11__ )
#define __RENDER_DX11__
// Required Headers
#include "Engine/Core/Win32Includes.hpp"
#define INITGUID
#include <d3d11.h>  
#include <DXGI.h>    

// DEBUG STUFF

#include <dxgidebug.h>
//#pragma comment( lib, "dxguid.lib" )

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "DXGI.lib" )

#include <d3dcompiler.h>
#pragma comment( lib, "d3dcompiler.lib" )

#define DX_SAFE_RELEASE(dx_resource)   if ((dx_resource) != nullptr) { dx_resource->Release(); dx_resource = nullptr; }

extern ID3D11Device *gD3DDevice;
extern ID3D11DeviceContext *gD3DContext;
extern IDXGISwapChain *gD3DSwapChain;


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
inline bool D3D11Setup(HWND hwnd, int windowHeight, int windowWidth)
{
	unsigned int device_flags = 0U;
#if defined(_DEBUG)
	device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Setup our Swap Chain
	DXGI_SWAP_CHAIN_DESC swapDescription;
	memset(&swapDescription, 0, sizeof(swapDescription));

	// fill the swap chain description struct
	swapDescription.BufferCount = 2; // two buffers (one front, one back?)
	swapDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_BACK_BUFFER;
	swapDescription.OutputWindow = hwnd; // the window to be copied to on present
	swapDescription.SampleDesc.Count = 1;  // how many multisamples (1 means no multi sampling)
	swapDescription.Windowed = TRUE;   // windowed/full-screen mode
	swapDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // use 32-bit color
	swapDescription.BufferDesc.Width = windowWidth;
	swapDescription.BufferDesc.Height = windowHeight;


	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(nullptr, // Adapter, if nullptr, will use adapter window is primarily on.
		D3D_DRIVER_TYPE_HARDWARE,  // Driver Type - We want to use the GPU (HARDWARE)
		nullptr,                   // Software Module - DLL that implements software mode (we do not use)
		device_flags,              // device creation options
		nullptr,                   // feature level (use default)
		0U,                        // number of feature levels to attempt
		D3D11_SDK_VERSION,         // SDK Version to use
		&swapDescription,                // Description of our swap chain
		&gD3DSwapChain,            // Swap Chain we're creating
		&gD3DDevice,               // [out] The device created
		nullptr,                   // [out] Feature Level Acquired
		&gD3DContext);            // Context that can issue commands on this pipe.

	return SUCCEEDED(hr);
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
inline void D3D11Cleanup()
{
	DX_SAFE_RELEASE(gD3DSwapChain);
	DX_SAFE_RELEASE(gD3DContext);
	DX_SAFE_RELEASE(gD3DDevice);
}


#endif 