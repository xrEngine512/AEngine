#pragma once
//
// Created by islam on 01.10.17.
//

#include "RuntimeBufferInfo.h"
#include <msgpack.hpp>

namespace ASL {
    enum ShaderType : char { NONE, VS, PS, GS, CS, DS, HS };

    struct ShaderCommon {
        ShaderType                      type        = NONE;
        std::string                     entry_point = "";
        std::vector<RuntimeBufferInfo>  buffers_info;
    };
}

MSGPACK_ADD_ENUM(ASL::ShaderType)
