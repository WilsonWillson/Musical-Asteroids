#include "Engine/Input/XboxControl.hpp"
#include "math.h"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Win32Includes.hpp"
#include <Xinput.h> // include the Xinput API
#pragma comment( lib, "xinput9_1_0" ) // Link in the xinput.lib static library // #Eiserloh: Xinput 1_4 doesn't work in Windows 7; using 9_1_0 explicitly
XINPUT_STATE xboxControllerState;


XboxControl::XboxControl()
	:m_x(0.f),
	m_y(0.f),
	m_leftDegrees(0.f),
	m_leftRadius(0.f),
	m_rightDegrees(0.f),
	m_rightRadius(0.f),
	m_leftVibration(0),
	m_rightVibration(0)

{
	for (int buttonIndex = 0; buttonIndex < AMOUNT_OF_BUTTONS; ++buttonIndex)
	{
		m_isButtonDown[buttonIndex] = false;
		m_wasButtonDown[buttonIndex] = false;
	}
}

XboxControl::~XboxControl()
{
}

void XboxControl::UpdateController(int controllerNum) {
	memset(&xboxControllerState, controllerNum, sizeof(xboxControllerState));
	DWORD errorStatus = XInputGetState(controllerNum, &xboxControllerState);
	if (errorStatus != ERROR_DEVICE_NOT_CONNECTED) {
		XInputGetState(controllerNum, &xboxControllerState);
		updateWasButtonDown();
		UpdateControlSticks();
		UpdateButtonsDown();
		UpdateVibration();
	}
}

void XboxControl::UpdateControlSticks() {
	UpdateLeftStickStatus();
	UpdateLeftControlStickPosition();
	UpdateRightStickStatus();
	UpdateRightControlStickPosition();
}
void XboxControl::UpdateButtonsDown() {
	UpdateStartButtonStatus();
	UpdateBackButtonStatus();
	UpdateAButtonStatus();
	UpdateBButtonStatus();
	UpdateXButtonStatus();
	UpdateYButtonStatus();
	UpdateLeftBumperStatus();
	UpdateRightBumperStatus();
	UpdateLeftTriggerStatus();
	UpdateRightTriggerStatus();
}

void XboxControl::updateWasButtonDown()
{
	for (int buttonIndex = 0; buttonIndex < AMOUNT_OF_BUTTONS; ++buttonIndex)
	{
		m_wasButtonDown[buttonIndex] = m_isButtonDown[buttonIndex];
	}
}


void XboxControl::UpdateStartButtonStatus() {
	m_wasButtonDown[START_BUTTON] = m_isButtonDown[START_BUTTON];
	m_isButtonDown[START_BUTTON] = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
}

void XboxControl::UpdateBackButtonStatus() {
	m_wasButtonDown[BACK_BUTTON] = m_isButtonDown[BACK_BUTTON];
	m_isButtonDown[BACK_BUTTON] = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
}


void XboxControl::UpdateAButtonStatus() {
	m_wasButtonDown[A_BUTTON] = m_isButtonDown[A_BUTTON];
	m_isButtonDown[A_BUTTON]= (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A) !=0;
}

void XboxControl::UpdateBButtonStatus() {
	m_wasButtonDown[B_BUTTON] = m_isButtonDown[B_BUTTON];
	m_isButtonDown[B_BUTTON] = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
}

void XboxControl::UpdateXButtonStatus() {
	m_wasButtonDown[X_BUTTON] = m_isButtonDown[X_BUTTON];
	m_isButtonDown[X_BUTTON] = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
}

void XboxControl::UpdateYButtonStatus() {
	m_wasButtonDown[Y_BUTTON] = m_isButtonDown[Y_BUTTON];
	m_isButtonDown[Y_BUTTON] = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
}


void XboxControl::UpdateLeftBumperStatus()
{
	m_wasButtonDown[LB_BUTTON] = m_isButtonDown[LB_BUTTON];
	m_isButtonDown[LB_BUTTON] = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
}

void XboxControl::UpdateRightBumperStatus()
{
	m_wasButtonDown[RB_BUTTON] = m_isButtonDown[RB_BUTTON];
	m_isButtonDown[RB_BUTTON] = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
}


void XboxControl::UpdateLeftTriggerStatus()
{
	m_wasButtonDown[LEFT_TRIGGER] = m_isButtonDown[LEFT_TRIGGER];
	float leftTrigger = (float)xboxControllerState.Gamepad.bLeftTrigger;
	if (leftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		m_isButtonDown[LEFT_TRIGGER] = true;
	if (leftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		m_isButtonDown[LEFT_TRIGGER] = false;
}

void XboxControl::UpdateRightTriggerStatus()
{
	m_wasButtonDown[RIGHT_TRIGGER] = m_isButtonDown[RIGHT_TRIGGER];
	float rightTrigger = (float)xboxControllerState.Gamepad.bRightTrigger;
	if (rightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		m_isButtonDown[RIGHT_TRIGGER] = true;
	if (rightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		m_isButtonDown[RIGHT_TRIGGER] = false;
}

void XboxControl::UpdateLeftStickStatus() {
	const float lowerNormalizedDeadZone = (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / 32768.f;
	if (m_leftRadius >= lowerNormalizedDeadZone){
		m_isButtonDown[LEFT_STICK] = true;
		return;
	}
	m_isButtonDown[LEFT_STICK] = false;
}

void XboxControl::UpdateLeftControlStickPosition() {
	const float lowerNormalizedDeadZone = (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / 32768.f;
	const float upperNormalizedDeadZone = .95f;
	Vector2 joyStickPosition;
	joyStickPosition.x = (float) xboxControllerState.Gamepad.sThumbLX;
	joyStickPosition.y = (float) xboxControllerState.Gamepad.sThumbLY;
	//Normalize currentXPosition and CurrentYPosition to turn to [-1,1]
	joyStickPosition.x /= 32768.f;
	joyStickPosition.y /= 32768.f;

	//Convert to Polar
	m_leftRadius = joyStickPosition.CalcLength();
	m_leftDegrees = joyStickPosition.CalcAngleDegrees();

	//clamp deadzone
	if (m_leftRadius <= lowerNormalizedDeadZone)
		m_leftRadius = 0.f;
	if (m_leftRadius >= upperNormalizedDeadZone)
		m_leftRadius = 1.f;

	//Range Map
	m_leftRadius = RangeMapFloat(m_leftRadius, lowerNormalizedDeadZone, upperNormalizedDeadZone, 0.f, 1.f);

	//figure out x and y
	m_x = m_leftRadius * CosDegrees(m_leftDegrees);
	m_y = m_leftRadius * SinDegrees(m_leftDegrees);
}

void XboxControl::UpdateRightStickStatus() {
	const float lowerNormalizedDeadZone = (float)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / 32768.f;
	if (m_rightRadius >= lowerNormalizedDeadZone) {
		m_isButtonDown[RIGHT_STICK] = true;
		return;
	}
	m_isButtonDown[RIGHT_STICK] = false;
}

void XboxControl::UpdateRightControlStickPosition() {
	const float lowerNormalizedDeadZone = (float)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / 32768.f;
	const float upperNormalizedDeadZone = .95f;
	Vector2 joyStickPosition;
	joyStickPosition.x = (float)xboxControllerState.Gamepad.sThumbRX;
	joyStickPosition.y = (float)xboxControllerState.Gamepad.sThumbRY;
	//Normalize currentXPosition and CurrentYPosition to turn to [-1,1]
	joyStickPosition.x /= 32768.f;
	joyStickPosition.y /= 32768.f;

	//Convert to Polar
	m_rightRadius = joyStickPosition.CalcLength();
	m_rightDegrees = joyStickPosition.CalcAngleDegrees();


	//clamp deadzone
	if (m_rightRadius <= lowerNormalizedDeadZone)
		m_rightRadius = 0.f;
	if (m_rightRadius >= upperNormalizedDeadZone)
		m_rightRadius = 1.f;
	//Range Map
	m_rightRadius = RangeMapFloat(m_rightRadius, lowerNormalizedDeadZone, upperNormalizedDeadZone, 0.f, 1.f);
	//DebuggerPrintf("Magnitude: %f ", m_radians);
	//figure out x and y
	m_x = m_rightRadius  * CosDegrees(m_rightDegrees);
	m_y = m_rightRadius  * SinDegrees(m_rightDegrees);
}

void XboxControl::UpdateVibration() {
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = (WORD)m_leftVibration; // use any value between 0-65535 here
	vibration.wRightMotorSpeed =(WORD)m_rightVibration; // use any value between 0-65535 here
	XInputSetState(0, &vibration);
}

void XboxControl::UpdateVibrationValue(float left, float right) {
	m_leftVibration = left;
	m_rightVibration = right;
}

void XboxControl::UpdateVibrationToAtLeastMinimumValue(float left, float right)
{
	if (m_leftVibration < left) {
		m_leftVibration = left;
	}
	if (m_rightVibration < right) {
		m_rightVibration = right;
	}
}

bool XboxControl::WasStartButtonJustPushed()
{
	if (m_wasButtonDown[START_BUTTON] == false && m_isButtonDown[START_BUTTON] == true)
		return true;
	return false;
}

bool XboxControl::WasBackButtonJustPushed()
{
	if (m_wasButtonDown[BACK_BUTTON] == false && m_isButtonDown[BACK_BUTTON] == true)
		return true;
	return false;
}

bool XboxControl::WasAButtonJustPushed()
{
	if (m_wasButtonDown[A_BUTTON] == false && m_isButtonDown[A_BUTTON] == true)
		return true;
	return false;
}

bool XboxControl::WasBButtonJustPushed()
{
	if (m_wasButtonDown[B_BUTTON] == false && m_isButtonDown[B_BUTTON] == true)
		return true;
	return false;
}

bool XboxControl::WasXButtonJustPushed()
{
	if (m_wasButtonDown[X_BUTTON] == false && m_isButtonDown[X_BUTTON] == true)
		return true;
	return false;
}

bool XboxControl::WasYButtonJustPushed()
{
	if (m_wasButtonDown[Y_BUTTON] == false && m_isButtonDown[Y_BUTTON] == true)
		return true;
	return false;
}


bool XboxControl::WasLeftBumperJustPushed()
{
	if (m_wasButtonDown[LB_BUTTON] == false && m_isButtonDown[LB_BUTTON] == true)
		return true;
	return false;
}

bool XboxControl::WasRightBumperJustPushed()
{
	if (m_wasButtonDown[RB_BUTTON] == false && m_isButtonDown[RB_BUTTON] == true)
		return true;
	return false;
}

bool XboxControl::WasLeftStickJustPushed()
{
	if (m_wasButtonDown[LEFT_STICK] == true && m_isButtonDown[LEFT_STICK] == false)
		return true;
	return false;
}


float XboxControl::GetLeftRadius() {
	return m_leftRadius;
}

float XboxControl::GetRightRadius() {
	return m_rightRadius;
}