#pragma once
//
// Created by islam on 01.10.17.
//

#include "ShaderCommon.h"
#include "ShaderParameterInfo.h"
#include "TextureInfo.h"

#include <msgpack.hpp>

namespace ASL {
    struct ShaderSource: ShaderCommon {
        std::string                         code;
        std::vector<ShaderParameterInfo>    parameters;
        std::vector<TextureInfo>            textures;

        MSGPACK_DEFINE(type, entry_point, buffers_info, code, parameters, textures)
    };
}


