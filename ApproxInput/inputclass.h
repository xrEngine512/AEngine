////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_
#include "IInternalInput.h"
#include "IExternalInput.h"
#include <atomic>
#include <EventEmitter.h>

enum ExitCode;

////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass : public IInternalInput, public IExternalInput
{
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

public:
	void Initialize() override;
    void SetWndCenter(int x, int y)override;
	static InputClass& Instance();
    LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM) override;
	bool IsKeyDown(unsigned int) override;
    bool IsIncrementalMouseControl() override;
	bool GetMouseCoord(float &x, float &y) override;
    bool GetMouseDelta(int &x, int &y) override;
	EventEmitter& BindToKey(UINT key)override final;
    IFnPtr* GetThreadFunc()override;
    ExitCode ThreadFunc();
private:
    HWND m_hwnd;
	bool m_keys[256];
	EventEmitter m_keyEvents[256];
    bool m_incControl;
    unsigned int m_x, m_y, m_wndPosXLocal, m_wndPosYLocal, m_wndPosXScreen, m_wndPosYScreen;
    std::atomic<int>  m_xDelta, m_yDelta;
    bool m_ignoreMovement;
};
InputClass* g_Input = &InputClass::Instance();


#endif