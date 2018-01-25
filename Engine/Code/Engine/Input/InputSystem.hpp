#pragma once
#include "Engine/Input/XboxControl.hpp"
#include "Engine/Input/KeyState.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Core/Win32Includes.hpp"
const int MAX_CONTROLLER = 4;
const int NUM_KEYS = 256;

enum KeyCode {
	MOUSE_LBUTTON = 0x01,
	MOUSE_RBUTTON = 0x02,
	KEY_BACKSPACE = 0x08,
	KEY_ENTER = 0x0D,
	KEY_TAB = 0x09,
	KEY_SHIFT = 0x10,
	KEY_ESCAPE = 0x1B,
	KEY_SPACE = 0x20,
	KEY_PAGEUP = 0x21,
	KEY_PAGEDOWN = 0x22,
	KEY_LEFTARROW = 0x25,
	KEY_UPARROW = 0x26,
	KEY_RIGHTARROW = 0x27,
	KEY_DOWNARROW = 0x28,
	KEY_0 = 0x30,
	KEY_1 = 0x31,
	KEY_2 = 0x32,
	KEY_3 = 0x33,
	KEY_4 = 0x34,
	KEY_5 = 0x35,
	KEY_6 = 0x36,
	KEY_7 = 0x37,
	KEY_8 = 0x38,
	KEY_9 = 0x39,
	KEY_A = 0x41,
	KEY_B = 0x42,
	KEY_C = 0x43,
	KEY_D = 0x44,
	KEY_E = 0x45,
	KEY_F = 0x46,
	KEY_G = 0x47,
	KEY_H = 0x48,
	KEY_I = 0x49,
	KEY_J = 0x4A,
	KEY_K = 0x4B,
	KEY_L = 0x4C,
	KEY_M = 0x4D,
	KEY_N = 0x4E,
	KEY_O = 0x4F,
	KEY_P = 0x50,
	KEY_Q = 0x51,
	KEY_R = 0x52,
	KEY_S = 0x53,
	KEY_T = 0x54,
	KEY_U = 0x55,
	KEY_V = 0x56,
	KEY_W = 0x57,
	KEY_X = 0x58,
	KEY_Y = 0x59,
	KEY_Z = 0x5A,
	KEY_F1 = 0x70,
	KEY_F2 = 0x71,
	KEY_F3 = 0x72,
	KEY_F4 = 0x73,
	KEY_F5 = 0x74,
	KEY_F6 = 0x75,
	KEY_F7 = 0x76,
	KEY_F8 = 0x77,
	KEY_F9 = 0x78,
	KEY_F10 = 0x79,
	KEY_F11 = 0x7A,
	KEY_F12 = 0x7B,
	Key_TILDE = 0xC0,
	KEY_LINE = 0xDC
};

class InputSystem
{
public:
	XboxControl m_controllers[MAX_CONTROLLER];
	KeyState	m_keyStates[NUM_KEYS];
	bool		m_DoesAppHasFocus = true;
	bool		m_IsMouseCursorHidden= false;
	bool		m_MouseWheelPositiveScoll = false;
	Vector2		m_previousMouseCords;
	Vector2		m_currentMouseCords;
	bool		m_IsMouseDown;

	
public:
	InputSystem();
	~InputSystem();
	void Update(HWND hwnd);
	bool IsKeyDown(int keyCode);
	bool WasKeyJustPressed(int keyCode);
	bool WasAnyKeyJustPressed(int keyCode);
	void OnKeyDown(int keyCode);
	void OnKeyUp(int keyCode);
	void LeftMouseButtonDown(bool IsDown);

	void setCurrentMoustCoords(HWND hwnd);
	Vector2 GetMouseScreenCoords(HWND hwnd);
	Vector2 GetMouseScreenCoords();
	void SetMouseScreenCoords(Vector2& newMouseScreenCoords);
	Vector2 GetDifferenceBetweenPreviousAndCurrentMouseCords();

	bool IsButtonDown(int controllerNum, int keyCode);
	bool wasButtonJustPressed(int controllerNum, int keyCode);
	void setKeyDown(int keyCode);

	Vector2 GetScreenSize();
	void OnAppGainedFocus();
	void OnAppLostFocus();
	bool DoesAppHaveFocus();

	void ShowMouseCursor();
	void HideMouseCursor();
	void SetMouseCursorHiddenWhenWeAreFocused(bool hideMouseWhenNotFocused);

private:
	void runMessagePump();
	void UpdateController();
	void UpdateKeyboard();
	void MouseWheelScroll();
};

extern InputSystem* m_inputSystem;