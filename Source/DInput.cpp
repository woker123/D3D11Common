#include "DInput.h"
#include <assert.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

DInput::DInput(HINSTANCE hInstance, HWND hwnd)
	:m_keyState{}, m_mouseState{}, m_isDetectingInput(false), m_deltaTime(0.0)
{
	bool init_D3DInput =
		initDInput(hInstance, hwnd);
	assert(init_D3DInput);
}

DInput::~DInput()
{
	shutdownDInput();
}

void DInput::detectInput(float deltaTime)
{
	//debut only
	assert(m_isDetectingInput = true);

	m_deltaTime = deltaTime;

	//get keyboard state
	HRESULT result =
		m_keyDevice->GetDeviceState(sizeof(m_keyState), m_keyState);
	if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
	{
		m_keyDevice->Acquire();
	}

	//get mouse state
	result =
		m_mouseDevice->GetDeviceState(sizeof(m_mouseState), &m_mouseState);
	if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
	{
		m_mouseDevice->Acquire();
	}
}

PRESS_STATE DInput::getKeyboardState(KEY_TYPE key)
{
	assert(m_isDetectingInput);
	return m_keyState[(int)key] ? PRESS_STATE::PRESS_DOWN : PRESS_STATE::PRESS_UP;
}

PRESS_STATE DInput::getMouseButtonState(MOUSE_BUTTON_TYPE mouseButton)
{
	assert(m_isDetectingInput);
	return ((unsigned char*)&m_mouseState)[(int)mouseButton] ? PRESS_STATE::PRESS_DOWN : PRESS_STATE::PRESS_UP;
}

MouseXYZ DInput::getMouseXYZSpeed()
{
	assert(m_isDetectingInput);
	MouseXYZ mxyz;
	mxyz.x = (float)m_mouseState.lX / m_deltaTime;
	mxyz.y = (float)m_mouseState.lY / m_deltaTime;
	mxyz.z = (float)m_mouseState.lZ / m_deltaTime;

	return mxyz;
}

MouseXYZ DInput::getMouseXYZPosition()
{
	assert(m_isDetectingInput);
	static MouseXYZ mpos = {};
	mpos.x += (float)m_mouseState.lX;
	mpos.y += (float)m_mouseState.lY;
	mpos.z += (float)m_mouseState.lZ;

	return mpos;
}

bool DInput::initKeyDevice(HWND hwnd)
{
	HRESULT result =
		m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyDevice, NULL);
	if (FAILED(result))
		return false;

	m_keyDevice->SetDataFormat(&c_dfDIKeyboard);
	m_keyDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);

	result = m_keyDevice->Acquire();

	return true;
}

bool DInput::initMouseDevice(HWND hwnd)
{
	HRESULT result =
		m_directInput->CreateDevice(GUID_SysMouse, &m_mouseDevice, NULL);
	if (FAILED(result))
		return false;

	m_mouseDevice->SetDataFormat(&c_dfDIMouse);
	m_mouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	result = m_mouseDevice->Acquire();

	return true;
}

bool DInput::initDInput(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result =
		DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, &m_directInput, NULL);

	if (!initKeyDevice(hwnd))
		return false;
	if (!initMouseDevice(hwnd))
		return false;

	return true;
}

void DInput::shutdownDInput()
{
	m_keyDevice->Unacquire();
	m_mouseDevice->Unacquire();
}
