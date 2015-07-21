#pragma once
#include "IExternalRenderObject.h"
struct ShaderDesc;

namespace ASL{
	__interface IApproxShaderLabExternalControl;
}

__interface IExternalScene
{
    IExternalRenderObject* AddRenderObject(char *modelFileName, const ShaderDesc& type, float PosX = 0, float PosY = 0, float PosZ = 0, float RotX = 0, float RotY = 0, float RotZ = 0);
    IExternalRenderObject* GetObjectByID(unsigned int ID);
    void GetCameraPos(float &x, float &y, float &z);
	void SetASLInterface(ASL::IApproxShaderLabExternalControl*);
};