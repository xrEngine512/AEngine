#pragma once 
#include "IExternalScene.h"

__interface IExternalRenderer
{
    void SetPosition(int x, int y);
    void GetWndSize(int& screenWidth, int& screenHeight);
    void SetInputFocus();
    IExternalScene* GetScene();
};