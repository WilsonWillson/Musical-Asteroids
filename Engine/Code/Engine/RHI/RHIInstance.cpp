#include "Engine/RHI/RHIInstance.hpp"
#include "Engine/Core/Windows.hpp"



RHIInstance* RHIInstance::sInstance = nullptr;

RHIInstance* RHIInstance::GetInstance()
{
	if (sInstance == nullptr) 
		new RHIInstance();

	return sInstance;
}

RHIInstance::RHIInstance()
{
	// Create the lowest level object - this is what everthing else comes from
	// Debug Setup
	m_debugModule = nullptr;
	m_debug = nullptr;
//#if 0
#if defined(_X64)
	m_debugModule = ::LoadLibrary("dxgidebug.dll");
#else 
	m_debugModule = LoadLibraryExA("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
#endif
	typedef HRESULT(WINAPI*GetDebugModuleCB)(REFIID, void**);
	GetDebugModuleCB cb = (GetDebugModuleCB) ::GetProcAddress(m_debugModule, "DXGIGetDebugInterface");
	cb(__uuidof(IDXGIDebug), (void**)&m_debug);

	m_debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
//#endif
	sInstance = this;
}

RHIInstance::~RHIInstance()
{
	sInstance = nullptr;

	// Debug Shutdown
//#if 0
	m_debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);

	DX_SAFE_RELEASE(m_debug);
	if (nullptr != m_debugModule) {
		FreeLibrary(m_debugModule);
		m_debugModule = nullptr;
	}
//#endif
}

void RHIInstance::ReportLiveObject()
{
	RHIInstance* instance = GetInstance();
	instance->m_debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
}

bool RHIInstance::CreateOutput(HINSTANCE applicationInstanceHandle, WNDCLASSEX windowClassDescription, RHIDevice*& device, RHIDeviceContext*& context, RHIOutput*& output)
{
	int windowWidth = 0;
	int windowHeight = 0;
	char* appName = "None";
	bool windowedOrNot = true;
	bool fullScreen = false;

	//Set Window configs
	setWindowHeightAndWidthInEngine(windowWidth, windowHeight);
	setWindowNameInEngine(appName);
	setWindowModeInEngine(windowedOrNot);
	setFullscreeninEngine(fullScreen);


	CreateDX11Window(applicationInstanceHandle, windowClassDescription, windowHeight, windowWidth, appName, windowedOrNot, fullScreen, device, context, output);
	return true;
}



