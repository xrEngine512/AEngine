////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_
#include <IInternalInput.h>
#include <IExternalInput.h>

#include <EventEmitter.h>
#include <MState.h>

#include <atomic>


class InputClass : public IInternalInput, public IExternalInput, public MActivityState
{
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

public:
	void Initialize() override;
    void SetWndCenter(int x, int y) override;
	static InputClass& Instance();

    void onKeyDown(int key) override final;
    void onKeyUp(int key) override final;
    void onMouseDown(int key) override final;
    void onMouseUp(int key) override final;
	bool IsKeyDown(int) override;
    bool IsIncrementalMouseControl() override;
	bool GetMouseCoord(float &x, float &y) override;
    bool GetMouseDelta(int &x, int &y) override;
	EventEmitter& BindToKey(int key) override final;
    Activity get_activity() override;
	ExitCode activity();
private:
    void* m_hwnd;
	bool m_keys[256];
	EventEmitter m_keyEvents[256];
    bool m_incControl;
    int m_x, m_y, m_wndPosXLocal, m_wndPosYLocal, m_wndPosXScreen, m_wndPosYScreen;
    std::atomic<int>  m_xDelta, m_yDelta;
    bool m_ignoreMovement;
};
InputClass* g_Input = &InputClass::Instance();


#endif