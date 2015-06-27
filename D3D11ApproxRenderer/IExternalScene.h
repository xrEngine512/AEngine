#pragma once
#include "ShaderTypes.h"
#include "IExternalRenderObject.h"

__interface IApproxShaderLabExternalControl;

__interface IExternalScene
{
    IExternalRenderObject* AddRenderObject(char *modelFileName, ShaderType type, float PosX = 0, float PosY = 0, float PosZ = 0, float RotX = 0, float RotY = 0, float RotZ = 0);
    IExternalRenderObject* GetObjectByID(unsigned int ID);
    void GetCameraPos(float &x, float &y, float &z);
    void SetASLInterface(IApproxShaderLabExternalControl*);
};