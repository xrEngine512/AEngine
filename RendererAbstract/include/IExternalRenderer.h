#pragma once 
#include "IExternalScene.h"
#include <vector>

interface IExternalRenderer
{
	virtual std::vector<ShaderDesc> AvailableShadersDesc() const = 0;
	virtual IExternalScene* get_scene() const = 0;
	virtual ~IExternalRenderer(){};
};