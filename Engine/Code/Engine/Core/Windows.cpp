#include "Engine/Core/Windows.hpp"

int OFFSET_FROM_WINDOWS_DESKTOP = 50;
HWND g_hWnd = nullptr;
HDC g_displayDeviceContext = nullptr;
HGLRC g_openGLRenderingContext = nullptr;