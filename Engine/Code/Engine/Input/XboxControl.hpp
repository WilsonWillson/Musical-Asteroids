#pragma  once

const int  AMOUNT_OF_BUTTONS = 19;
const int  START_BUTTON = 0;
const int  BACK_BUTTON = 1;
const int  LEFT_STICK = 2;
const int  RIGHT_STICK = 3;
const int  A_BUTTON = 4;
const int  B_BUTTON = 5;
const int  X_BUTTON = 6;
const int  Y_BUTTON = 7;
const int  LB_BUTTON = 8;
const int  RB_BUTTON = 9;
const int LEFT_TRIGGER = 10;
const int RIGHT_TRIGGER = 11;


class XboxControl
{
public:
	XboxControl();
	~XboxControl();

	void UpdateController(int controllerNum);
	void UpdateButtonsDown();
	void UpdateControlSticks();
	void updateWasButtonDown();

	void UpdateAButtonStatus();
	void UpdateBButtonStatus();
	void UpdateXButtonStatus();
	void UpdateYButtonStatus();
	void UpdateLeftBumperStatus();
	void UpdateRightBumperStatus();
	void UpdateLeftTriggerStatus();
	void UpdateRightTriggerStatus();
	void UpdateStartButtonStatus();
	void UpdateBackButtonStatus();

	void UpdateLeftStickStatus();
	void UpdateLeftControlStickPosition();
	void UpdateRightStickStatus();
	void UpdateRightControlStickPosition();

	void UpdateVibration();
	void UpdateVibrationValue(float left, float right);
	void UpdateVibrationToAtLeastMinimumValue(float left, float right);

	bool WasStartButtonJustPushed();
	bool WasBackButtonJustPushed();
	bool WasAButtonJustPushed();
	bool WasBButtonJustPushed();
	bool WasXButtonJustPushed();
	bool WasYButtonJustPushed();
	bool WasLeftBumperJustPushed();
	bool WasRightBumperJustPushed();
	bool WasLeftStickJustPushed();

	float GetLeftRadius();
	float GetRightRadius();
public:
	float	 m_x;
	float	 m_y;
	float	 m_leftDegrees;
	float	 m_rightDegrees;
	float	 m_leftRadius;
	float	 m_rightRadius;
	float	 m_leftVibration;
	float	m_rightVibration;
	bool	m_isButtonDown[AMOUNT_OF_BUTTONS];
	bool	m_wasButtonDown[AMOUNT_OF_BUTTONS];
};
