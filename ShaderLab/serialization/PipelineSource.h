#pragma once
//
// Created by islam on 01.10.17.
//


#include "RuntimeBufferInfo.h"
#include "ShaderSource.h"

namespace ASL {
    struct PipelineSource {
        RuntimeBufferInfo           info;
        std::vector<ShaderSource>   shaders;

        const ShaderSource & get_shader_by_type(ShaderType type) const
        {
            for (auto & shader : shaders)
            {
                if (shader.type == type)
                {
                    return shader;
                }
            }
            throw std::runtime_error("shader with given type not found");
        }

        ShaderSource & get_shader_by_index(int index)
        {
            if (index < shaders.size()) {
                return shaders[index];
            }
            throw std::runtime_error("shader with given type not found");
        }

        MSGPACK_DEFINE(info, shaders);
    };
}


