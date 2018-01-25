#pragma once
#if !defined( __RENDER_INSTANCE__ )
#define __RENDER_INSTANCE__

#include "Engine/Core/Win32Includes.hpp"
#include <dxgidebug.h>

#include "Engine/DX11/DX11.hpp"
#include "Engine/Core/EngineConfig.hpp"

class RHIInstance;  // System level singleton
class RHIDevice;     // physical GPU
class RHIDeviceContext;
class RHIOutput;

class RHIInstance
{
public:
	static RHIInstance* GetInstance();
	~RHIInstance();

	static void ReportLiveObject();
	bool CreateOutput(HINSTANCE applicationInstanceHandle, WNDCLASSEX windowClassDescription, RHIDevice*& device, RHIDeviceContext*& context, RHIOutput*& output);
	static RHIInstance* sInstance;

private:
	RHIInstance();

public:
	HMODULE m_debugModule;
	IDXGIDebug *m_debug;
};
#endif