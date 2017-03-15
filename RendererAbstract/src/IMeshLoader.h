#pragma once
#include "IGeometryBuffer.h"

#include <experimental/string_view>

interface IMeshLoader
{
    virtual IGeometryBufferPtr Load(const std::experimental::string_view &meshFileName) = 0;
    virtual ~IMeshLoader(){};
};

IMeshLoader& getMeshLoader();
