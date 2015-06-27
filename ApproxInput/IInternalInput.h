#pragma once
#include "WindowsKeys.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
__interface IFnPtr;

__interface IInternalInput
{
    void Initialize();
    void SetWndCenter(int x, int y);
    LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);  
    IFnPtr* GetThreadFunc();
};

 extern "C" _declspec(dllexport) IInternalInput* GetInput();
