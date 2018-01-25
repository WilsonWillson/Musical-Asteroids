#pragma once
#if !defined(  ___WINDOW_OUTPUT__ )
#define ___WINDOW_OUTPUT__
#include <string>
#include "Engine/DX11/DX11.hpp"
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/RHI/RHIDeviceContext.hpp"
#include "Engine/RHI/RHIOutput.hpp"
#include "Engine/RHI/RHIInstance.hpp"

extern int OFFSET_FROM_WINDOWS_DESKTOP;
extern HWND g_hWnd;
extern HDC g_displayDeviceContext;
extern HGLRC g_openGLRenderingContext;

//------------------------------------Create Window--------------------------------------------------------------

inline void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}



inline void CreateDX11Window(HINSTANCE applicationInstanceHandle, WNDCLASSEX windowClassDescription, int windowHeight, int windowWidth,
	                     char*& appName, bool WindowedOrNot, bool fullscreen, RHIDevice*& rhiDevice, RHIDeviceContext*& deviceContext,  RHIOutput*& rhiOutput)
{
	DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	if(WindowedOrNot == false)
		windowStyleFlags = WS_POPUP;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect(desktopWindowHandle, &desktopRect);

	int horizontal = 0;
	int vertical = 0;
	GetDesktopResolution(horizontal, vertical);
	
	RECT windowRect;
	if(fullscreen && WindowedOrNot == false)
		windowRect = { 0, 0, horizontal, vertical};
	else
	{
		//Center window in middle of screen if its not fullscreen
		GetWindowRect(desktopWindowHandle, &desktopRect);

		int xPos = (desktopRect.right) / 2;
		int yPos = (desktopRect.bottom) / 2;
		int HalfOfWindowWidth = windowWidth / 2;
		int HalfOfWidowHeight = windowHeight / 2;

		windowRect = { xPos - HalfOfWindowWidth, yPos - HalfOfWidowHeight, xPos + HalfOfWindowWidth, yPos + HalfOfWidowHeight };

	}
	AdjustWindowRectEx(&windowRect, windowStyleFlags, FALSE, windowStyleExFlags);

	WCHAR windowTitle[1024];
	MultiByteToWideChar(GetACP(), 0, appName, -1, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
	g_hWnd = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		applicationInstanceHandle,
		NULL);

	ShowWindow(g_hWnd, SW_SHOW);
	SetForegroundWindow(g_hWnd);
	SetFocus(g_hWnd);

	g_displayDeviceContext = GetDC(g_hWnd);

	HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
	SetCursor(cursor);

	RECT currentWindow;
	GetWindowRect(g_hWnd, &currentWindow);
	ClipCursor(&currentWindow);

	D3D11Setup( g_hWnd, windowHeight, windowWidth);


    RHIInstance *ri = RHIInstance::GetInstance();
	RHIDevice *device = new RHIDevice(ri, gD3DDevice);
	RHIDeviceContext *context = new RHIDeviceContext(device, gD3DContext);
	RHIOutput *output = new RHIOutput(device, gD3DSwapChain, nullptr);

	device->m_immediateContext = context;

	rhiDevice = device;
	deviceContext = context;
	rhiOutput = output;
}

//-------------------RESET SCREEN---------------------------------------------

inline void cleanScreen(float clear_color[])
{
	// Get the back buffer
	ID3D11Texture2D *back_buffer = nullptr;
	gD3DSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);

	// Get a render target view of this
	ID3D11RenderTargetView *rtv = nullptr;
	gD3DDevice->CreateRenderTargetView(back_buffer, nullptr, &rtv); //MEMORY LEAK HERE
	DX_SAFE_RELEASE(back_buffer); 

	gD3DContext->ClearRenderTargetView(rtv, clear_color);
	DX_SAFE_RELEASE(rtv); 

}


//------------------------------UPDATE SCREEN-----------------------------------
inline void pump()
{
	// LIKE SWAP BUFFER
	gD3DSwapChain->Present(0, 0); 
}


#endif