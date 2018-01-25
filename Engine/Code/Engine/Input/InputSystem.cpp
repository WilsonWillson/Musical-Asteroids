#include "Engine/Input/InputSystem.hpp"
#include <string.h>
#include "Engine/Core/Win32Includes.hpp"

void InputSystem::LeftMouseButtonDown(bool IsDown)
{
	if (IsDown == true)
	{
		m_keyStates[MOUSE_LBUTTON].m_isDown = true;
	}

	if (IsDown = false)
	{
		m_keyStates[MOUSE_LBUTTON].m_isDown = false;
	}
}

InputSystem::InputSystem()
{
	memset(m_keyStates, 0, sizeof(m_keyStates));

	for (int keyIndex = 0; keyIndex < NUM_KEYS; keyIndex++) {
		m_keyStates[keyIndex].m_wasDown = false;
		m_keyStates[keyIndex].m_isDown = false;
	}
	m_IsMouseDown = false;
}

InputSystem::~InputSystem()
{

}

void InputSystem::Update(HWND hwnd)
{
	UpdateController();
	UpdateKeyboard();
	setCurrentMoustCoords(hwnd);
	//SetCursorPos();
	runMessagePump();
}

bool InputSystem::IsKeyDown(int keyCode)
{
	return m_keyStates[keyCode].m_isDown;
}

bool InputSystem::WasKeyJustPressed(int keyCode)
{
	if (m_keyStates[keyCode].m_wasDown == false && m_keyStates[keyCode].m_isDown == true)
		return true;
	return false;
}

bool InputSystem::WasAnyKeyJustPressed(int keyCode)
{
	for (int keyIndex = 0; keyIndex < NUM_KEYS; keyIndex++) 
	{
		if (m_keyStates[keyCode].m_wasDown == false && m_keyStates[keyCode].m_isDown == true)
			return true;
	}
	return false;
}


void InputSystem::OnKeyDown(int keyCode)
{
	m_keyStates[keyCode].m_isDown = true;
}

void InputSystem::OnKeyUp(int keyCode)
{
	m_keyStates[keyCode].m_isDown = false;
}

void InputSystem::setCurrentMoustCoords(HWND hwnd)
{
	m_previousMouseCords = m_currentMouseCords;
	m_currentMouseCords = GetMouseScreenCoords(hwnd);
}

Vector2 InputSystem::GetMouseScreenCoords(HWND hwnd)
{
	POINT cursorPoint;
	GetCursorPos(&cursorPoint);
	ScreenToClient(hwnd, &cursorPoint);
	Vector2 cursorScreenPosFloats((float)cursorPoint.x, (float)cursorPoint.y);
	return cursorScreenPosFloats;
}

Vector2 InputSystem::GetMouseScreenCoords()
{
	POINT cursorPoint;
	GetCursorPos(&cursorPoint);
	Vector2 cursorScreenPosFloats((float)cursorPoint.x, (float)cursorPoint.y);
	return cursorScreenPosFloats;
}

void InputSystem::SetMouseScreenCoords(Vector2& newMouseScreenCoords)
{
	int x = (int)newMouseScreenCoords.x;
	int y = (int)newMouseScreenCoords.y;
	SetCursorPos(x, y);
}

Vector2 InputSystem::GetDifferenceBetweenPreviousAndCurrentMouseCords()
{
	return m_currentMouseCords - m_previousMouseCords;
}

bool InputSystem::IsButtonDown(int controllerNum, int keyCode)
{
	if (m_controllers[controllerNum].m_isButtonDown[keyCode] == true)
		return true;
	return false;
}

bool InputSystem::wasButtonJustPressed(int controllerNum, int keyCode)
{
	if (m_controllers[controllerNum].m_wasButtonDown[keyCode] == false && m_controllers[0].m_isButtonDown[keyCode] == true)
		return true;
	return false;
}

void InputSystem::runMessagePump()
{
	MSG queuedMessage;
	for (;; )
	{
		const BOOL wasMessagePresent = PeekMessage(&queuedMessage, NULL, 0, 0, PM_REMOVE);
		if (!wasMessagePresent)
		{
			break;
		}

		TranslateMessage(&queuedMessage);
		DispatchMessage(&queuedMessage);
	}
}

void InputSystem::UpdateController()
{
	for (int controllerIndex = 0; controllerIndex < MAX_CONTROLLER; ++controllerIndex)
	{
		m_controllers[controllerIndex].UpdateController(controllerIndex);
	}
}

void InputSystem::UpdateKeyboard()
{
	if (m_keyStates[MOUSE_LBUTTON].m_wasDown == true)
		m_keyStates[MOUSE_LBUTTON].m_isDown = false;
	
	for (int keyIndex = 0; keyIndex < NUM_KEYS; ++keyIndex)
	{
		m_keyStates[keyIndex].m_wasDown = m_keyStates[keyIndex].m_isDown;
	}
}


void InputSystem::setKeyDown(int keyCode)
{
	OnKeyDown(keyCode);
}

Vector2 InputSystem::GetScreenSize()
{
	HWND desktopWindowHandle = GetDesktopWindow();
	RECT desktopRect;
	GetWindowRect(desktopWindowHandle, &desktopRect);
	Vector2 screenSize;
	screenSize.x = (float)(desktopRect.right - desktopRect.left);
	screenSize.y = (float)(desktopRect.bottom - desktopRect.top); 
	return screenSize;
}

void InputSystem::OnAppGainedFocus()
{
	m_DoesAppHasFocus = true;
	if (m_IsMouseCursorHidden)
		HideMouseCursor();
}

void InputSystem::OnAppLostFocus()
{
	m_DoesAppHasFocus = false;
	if (m_IsMouseCursorHidden)
		ShowMouseCursor();
}

bool InputSystem::DoesAppHaveFocus()
{
	return m_DoesAppHasFocus;
}


void InputSystem::ShowMouseCursor()
{
	for (;;)
	{
		int newCursorShowValue = ShowCursor(TRUE);
		if (newCursorShowValue >= 0)
			break;
	}
}


void InputSystem::HideMouseCursor()
{
	for (;;)
	{
		int newCursorShowValue = ShowCursor(FALSE);
		if (newCursorShowValue < 0)
			break;
	}
}

void InputSystem::SetMouseCursorHiddenWhenWeAreFocused(bool hideMouseWhenNotFocused)
{
	m_IsMouseCursorHidden = hideMouseWhenNotFocused;
	if (m_IsMouseCursorHidden && m_DoesAppHasFocus)
		HideMouseCursor();
	else
		ShowMouseCursor();
}

