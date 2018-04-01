#pragma once
//
// Created by islam on 01.10.17.
//


#include "RuntimeBufferInfo.h"
#include "ShaderCompiled.h"

namespace ASL {

    struct PipelineCompiled {
        std::vector<ShaderCompiled> shaders;

        MSGPACK_DEFINE(shaders);
    };
}



