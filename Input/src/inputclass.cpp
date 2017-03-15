////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"
#include "Keys.h"

#include <IO.h>


using namespace std;


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

//InputClass* g_Input = &InputClass::instance();

void InputClass::Initialize()
{
	int i;

	// initialize all the keys to being released and not pressed.
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

void InputClass::onKeyDown(int key)
{
	// If a key is pressed then save that state in the key array.
	m_keys[key] = true;
    m_keyEvents[key]();
    if (key == G_Key)
    {
        m_incControl = !m_incControl;
        IO::Mouse::set_cursor_visible(!m_incControl);
    }
	return;
}


void InputClass::onKeyUp(int key)
{
	// If a key is released then clear that state in the key array.
	m_keys[key] = false;
	return;
}

bool InputClass::IsIncrementalMouseControl()
{
    return m_incControl;
}

bool InputClass::IsKeyDown(int key)
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

EventEmitter& InputClass::BindToKey(int key)
{
	return m_keyEvents[key];
}

Activity InputClass::get_activity()
{  
    return bind(&InputClass::activity, this);
}

ExitCode InputClass::activity()
{    
    auto pos = IO::Mouse::get_pointer_position();
    if (pos.x == -1 && pos.y == -1)
        return RUNNING;

    m_x = pos.x;
    m_y = pos.y;
    if (m_incControl)
    {
        m_xDelta.store(m_xDelta.load() + m_x - m_wndPosXScreen);
        m_yDelta.store(m_yDelta.load() + m_y - m_wndPosYScreen);
        IO::Mouse::set_pointer_position({m_wndPosXScreen, m_wndPosYScreen});
    }
    return RUNNING;
}

bool InputClass::GetMouseCoord(float &x, float &y)
{
	x = m_x;
	y = m_y;
    return !m_incControl;
}

void InputClass::onMouseDown(int key) {
    onKeyDown(key);
}

void InputClass::onMouseUp(int key) {
    onKeyUp(key);
}
