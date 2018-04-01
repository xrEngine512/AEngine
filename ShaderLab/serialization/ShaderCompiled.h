#pragma once
//
// Created by islam on 01.10.17.
//

#include "ShaderCommon.h"

#include <vector>
#include <cstdint>

#include <msgpack.hpp>

namespace ASL {
    struct ShaderCompiled: ShaderCommon {
        std::vector<uint32_t>   bytes;
        std::vector<int>        parameters_ids;
        std::vector<int>        texture_slots;

        MSGPACK_DEFINE(type, bytes, entry_point, buffers_info, parameters_ids, texture_slots)
    };
}


