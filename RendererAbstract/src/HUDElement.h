#pragma once
#include "RenderObject.h"
#include "IShader.h"
#include "IMeshLoader.h"

#include <ApproxMath.h>
#include <atomic>
#include <string>

interface IShaderParams;
interface ITexture2D;

class HUDElement : public RenderObject
{

public:
    HUDElement(const std::string& texture, unsigned int width, unsigned int height);
    virtual ~HUDElement();
};

