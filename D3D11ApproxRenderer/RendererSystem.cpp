#include "RendererSystem.h"
#include <IInternalInput.h>
#include <FnPtr.h>
#include "Scene.h"

RendererSystem::RendererSystem() :m_Scene(nullptr), m_SoundSys(nullptr)
{
    m_done.store(false);
}

RendererSystem& RendererSystem::Instance()
{
    static RendererSystem instance;
    return instance;
}

void RendererSystem::SetInputFocus()
{
    SetFocus(m_hwnd);
}

RendererSystem::~RendererSystem()
{
}

IFnPtr* RendererSystem::GetThreadFunc()
{
    return new FnPtr<RendererSystem>(&RendererSystem::Thread, this);
}

void RendererSystem::InitializeWindows(HWND parent_hwnd, int posX, int posY)
{
    WNDCLASSEX wc;
    DEVMODE dmScreenSettings;

    // Get the instance of this application.
    m_hinstance = GetModuleHandle(NULL);

    // Give the application a name.
    m_applicationName = L"Engine";

    // Setup the windows class with default settings.
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hinstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = m_applicationName;
    wc.cbSize = sizeof(WNDCLASSEX);

    // Register the window class.
    RegisterClassEx(&wc);

    // Determine the resolution of the clients desktop screen.
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Setup the screen settings depending on whether it is running in full screen or in windowed mode.
    if (m_FullScreenOn)
    {
        // If full screen set the screen to maximum size of the users desktop and 32bit.
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
        dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Change the display settings to full screen.
        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

    }
    else
    {
        // If windowed then set it to 800x600 resolution.
        screenWidth = m_screenWidth;
        screenHeight = m_screenHeight;
    }

    // Create the window with the screen settings and get the handle to it.
    m_hwnd = CreateWindowEx(WS_EX_TOOLWINDOW, m_applicationName, m_applicationName,
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        posX, posY, screenWidth, screenHeight, parent_hwnd, NULL, m_hinstance, NULL);

    // Bring the window up on the screen and set it as main focus.
    ShowWindow(m_hwnd, SW_SHOW);
    SetForegroundWindow(m_hwnd);
    SetFocus(m_hwnd);

    // Hide the mouse cursor.
    

    return;
}

void RendererSystem::GetWndSize(int& screenWidth, int& screenHeight)
{
    screenWidth = m_screenWidth;
    screenHeight = m_screenHeight;
}

bool RendererSystem::Initialize(void* parentWindowID, bool FullScreenOn, int screenWidth, int screenHeight, int xPos, int yPos)
{
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_FullScreenOn = FullScreenOn;
    HWND vp;
    if (!parentWindowID)
    {
        InitializeWindows((HWND)parentWindowID, xPos, yPos);
        vp = m_hwnd;
    }
    else
        vp = (HWND)parentWindowID;

    m_Scene = new Scene;
    if (!m_Scene)
    {
        return false;
    }
    
    
    return m_Scene->Initialize(vp, screenWidth, screenHeight, FullScreenOn);
}

ExitCode RendererSystem::Thread()
{
    MSG msg;
    bool result;

    // Initialize the message structure.
    ZeroMemory(&msg, sizeof(MSG));

    // Loop until there is a quit message from the window or the user.
    if (!m_done.load())
    {
        // Handle the windows messages.
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // If windows signals to end the application then exit out.
        if (msg.message == WM_QUIT)
        {
            m_done.store(true);
        }
        else
        {
            // Otherwise do the frame processing.
            result = Frame();
            if (!result)
            {
                m_done.store(true);
            }
        }
        return RUNNING;
    }
    return DONE;
}

bool RendererSystem::Frame()
{
    bool result;

    // Do the frame processing for the graphics object.

    result =  m_Scene->Frame();
    if (m_SoundSys)
        m_SoundSys->Update();
    if (!result)
    {
        return false;
    }

    return true;
}

void RendererSystem::BindSoundSystem(IInternalSound* s_sys)
{
    m_SoundSys = s_sys;
}

IExternalScene* RendererSystem::GetScene()
{
    return m_Scene;
}

void RendererSystem::SetPosition(int x, int y)
{
    SetWindowPos(m_hwnd, NULL, x, y, 0, 0, SWP_NOSIZE);
}

void RendererSystem::Shutdown()
{
    m_done.store(true);
     
    if (m_Scene)
    {
        m_Scene->Shutdown();
        delete m_Scene;
        m_Scene = nullptr;
    }
    return ShutdownWindows();
}

void RendererSystem::ShutdownWindows()
{
    // Show the mouse cursor.
    ShowCursor(true);

    // Fix the display settings if leaving full screen mode.
    if (m_FullScreenOn)
    {
        ChangeDisplaySettings(NULL, 0);
    }

    // Remove the window.
    DestroyWindow(m_hwnd);
    m_hwnd = NULL;

    // Remove the application instance.
    UnregisterClass(m_applicationName, m_hinstance);
    m_hinstance = NULL;

    return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
    switch (umessage)
    {
        // Check if the window is being destroyed.
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }

    // Check if the window is being closed.
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }

    // All other messages pass to external input system.
    default:
    {
        return GetInput()->MessageHandler(hwnd, umessage, wparam, lparam);
    }
    }
}