#pragma once
#include "IInternalRenderer.h"
#include "IExternalRenderer.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <atomic>
#include <IInternalSound.h>
#include <vector>

__interface IFnPtr;
enum ExitCode;
using namespace std;
class Scene;

class  RendererSystem : public IInternalRenderer, public IExternalRenderer
{
    Scene* m_Scene;
    atomic_bool m_done;
    bool m_FullScreenOn;
    int m_screenWidth;
    int m_screenHeight;
    LPCWSTR m_applicationName;
    HINSTANCE m_hinstance;
    HWND m_hwnd;
    IInternalSound* m_SoundSys;
    
    void InitializeWindows(HWND parent_hwnd, int posX, int posY);
    void ShutdownWindows();
    ExitCode Thread();
    bool Frame();
    RendererSystem();
    ~RendererSystem();
public:
	const vector<const ShaderDesc*>& AvailableShadersDesc()const override final;
	void SetInputFocus()override final;
	void BindSoundSystem(IInternalSound* s_sys)override final;
    static _declspec(dllexport) RendererSystem& Instance();
	bool Initialize(void* parentWindowID, bool FullScreenOn, int screenWidth, int screenHeight, int xPos, int yPos)override final;
	void Shutdown()override final;
	void SetPosition(int x, int y)override final;
    void GetWndSize(int& screenWidth, int& screenHeight)const override final;
	IFnPtr* GetThreadFunc()override final;
	IExternalScene* GetScene()const override final;
};

IInternalRenderer* GetRenderer()
{
    return  &RendererSystem::Instance();
}

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);