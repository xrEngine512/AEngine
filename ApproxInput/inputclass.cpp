////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"
#include <windowsx.h>
#include "WindowsKeys.h"
#include <stdlib.h>
#include "../ApproxCommonSystem/IFnPtr.h"
#include "../ApproxCommonSystem/FnPtr.h"


InputClass::InputClass() :m_incControl(false), m_wndPosXLocal(0), m_wndPosYLocal(0), m_wndPosXScreen(0), m_wndPosYScreen(0), m_ignoreMovement(false), m_hwnd(nullptr)
{
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}

IInternalInput* GetInput()
{
    return g_Input;
}

InputClass& InputClass::Instance()
{
	static InputClass instance;
	return instance;
}

//InputClass* g_Input = &InputClass::Instance();

void InputClass::Initialize()
{
	int i;

	// Initialize all the keys to being released and not pressed.
	for(i=0; i<256; i++)
	{
		m_keys[i] = false;
	}
	return;
}

void InputClass::SetWndCenter(int x, int y)
{
    m_wndPosXScreen = x;
    m_wndPosYScreen = y;
}

LRESULT CALLBACK InputClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    m_hwnd = hwnd;
    
    switch (umsg)
    {
        // Check if a key has been pressed on the keyboard.
    case WM_KEYDOWN:
    {
        // If a key is pressed send it to the input object so it can record that state.
        m_keys[(static_cast<unsigned int>(wparam))] = true;
		m_keyEvents[static_cast<unsigned int>(wparam)]();
        if (wparam == G_Key)
        {
            m_incControl = !m_incControl;
            ShowCursor(!m_incControl);
        }
        return 0;
    }

    case WM_LBUTTONDOWN:
    {
        m_keys[(static_cast<unsigned int>(VK_LBUTTON))] = true;
        return 0;
    }

    case WM_LBUTTONUP:
    {
        m_keys[(static_cast<unsigned int>(VK_LBUTTON))] = false;
        return 0;
    }

    case WM_RBUTTONDOWN:
    {
        m_keys[(static_cast<unsigned int>(VK_RBUTTON))] = true;
        return 0;
    }

    case WM_RBUTTONUP:
    {
        m_keys[(static_cast<unsigned int>(VK_RBUTTON))] = false;
        return 0;
    }

    // Check if a key has been released on the keyboard.
    case WM_KEYUP:
    {
        // If a key is released then send it to the input object so it can unset the state for that key.
        m_keys[(static_cast<unsigned int>(wparam))] = false;
        return 0;
    }
    case WM_MOUSEMOVE:
    {        
        Sleep(16);
        return 0;
    }

    // Any other messages send to the default message handler as our application won't make use of them.
    default:
    {
        //m_Input->MAllKeyUp();
        return 1;// DefWindowProc(hwnd, umsg, wparam, lparam);
    }
    }
}

/*void InputClass::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
	return;
}


void InputClass::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
	return;
}*/

bool InputClass::IsIncrementalMouseControl()
{
    return m_incControl;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}

bool InputClass::GetMouseDelta(int &x, int &y)
{
    y = m_xDelta.load();
    x = m_yDelta.load();
    m_xDelta.store(0);
    m_yDelta.store(0);
 
    return m_incControl;
}

EventEmitter& InputClass::BindToKey(UINT key)
{
	return m_keyEvents[key];
}

IFnPtr* InputClass::GetThreadFunc()
{  
    return new FnPtr<InputClass>(&InputClass::ThreadFunc, this);
}

ExitCode InputClass::ThreadFunc()
{    
    POINT pos;
    GetCursorPos(&pos);
    m_x = pos.x;
    m_y = pos.y;
    if (m_incControl)
    {
        m_xDelta.store(m_xDelta.load() + static_cast<int>(m_x)-static_cast<int>(m_wndPosXScreen));
        m_yDelta.store(m_yDelta.load() + static_cast<int>(m_y)-static_cast<int>(m_wndPosYScreen));
        SetCursorPos(m_wndPosXScreen, m_wndPosYScreen);
    }
    return RUNNING;
}

bool InputClass::GetMouseCoord(float &x, float &y)
{
	x = m_x;
	y = m_y;
    return !m_incControl;
}