#pragma once 
#include "IExternalScene.h"
#include <vector>

__interface IExternalRenderer
{
    void SetPosition(int x, int y);
    void GetWndSize(int& screenWidth, int& screenHeight)const;
    void SetInputFocus();
	const std::vector<const ShaderDesc*>& AvailableShadersDesc()const;
    IExternalScene* GetScene()const;
};