#pragma once

#include <Windows.h>
#include <dinput.h>
#include <wrl.h>

#define MWR Microsoft::WRL
#define ADD_KEY(KeyName) KEY_##KeyName = DIK_##KeyName

//flexible, keyboard enum, you can add your keyboard enum by using above macro
enum class KEY_TYPE
{
	ADD_KEY(W),
	ADD_KEY(A),
	ADD_KEY(S),
	ADD_KEY(D)

};


//fixed, mouse button enumeration
enum class MOUSE_BUTTON_TYPE
{
	BUTTON_LEFT = DIMOFS_BUTTON0,
	BUTTON_RIGHT = DIMOFS_BUTTON1,
	BUTTON_SCROLL = DIMOFS_BUTTON2
};


//fixed, see if the button is pressed
enum class PRESS_STATE
{
	PRESS_UP = 0,
	PRESS_DOWN = 1
};

//store mouse device axis information
struct MouseXYZ
{
	float x;
	float y;
	float z;
};


class GameInput
{
public:
	GameInput(HINSTANCE hInstance, HWND hwnd);
	~GameInput();
	GameInput(const GameInput&) = delete;
	GameInput& operator=(const GameInput&) = delete;


public:
	void detectInput(float deltaTime);

	/*retrieve device data functions*/
	PRESS_STATE getKeyboardState(KEY_TYPE key);
	PRESS_STATE getMouseButtonState(MOUSE_BUTTON_TYPE mouseButton);
	MouseXYZ getMouseXYZSpeed();
	MouseXYZ getMouseXYZPosition();

private:
	bool initKeyDevice(HWND hwnd);
	bool initMouseDevice(HWND hwnd);
	bool initGameInput(HINSTANCE hInstance, HWND hwnd);
	void shutdownGameInput();

private:
	MWR::ComPtr<IDirectInput8> m_directInput;
	MWR::ComPtr<IDirectInputDevice8> m_keyDevice;
	MWR::ComPtr<IDirectInputDevice8> m_mouseDevice;

	unsigned char m_keyState[256];
	DIMOUSESTATE m_mouseState;

	bool m_isDetectingInput;
	float m_deltaTime;
};

