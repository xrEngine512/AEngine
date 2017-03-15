#pragma once
#include "IExternalRenderObject.h"

#include <Promise.h>
#include <ApproxMath.h>

struct ShaderDesc;
typedef Promise<IExternalRenderObject*> RenderObjectPromise;

namespace ASL{
	interface IApproxShaderLabExternalControl;
}

interface IExternalScene
{
    virtual RenderObjectPromise AddRenderObject(const std::string &modelFileName, const ShaderDesc &type,
												const Math::AVector3 &position = {},
												const Math::AVector3 &orientation = {}) = 0;
	virtual IExternalRenderObject* GetObjectByID(unsigned int ID) = 0;
	virtual void GetCameraPos(float &x, float &y, float &z) = 0;
	virtual void SetASLInterface(ASL::IApproxShaderLabExternalControl*) = 0;
	virtual void SetFov(float) = 0;
	virtual void SetScreenWidth(float) = 0;
	virtual void SetScreenHeight(float) = 0;
	virtual ~IExternalScene(){};
};