#pragma once
#include <msgpack.hpp>

namespace ASL
{
	enum VarGroup : char { SCENE_CONSTANT, SHADER_PARAMS, PER_FRAME, PER_OBJECT };
    enum ShaderModel{ SM_NONE, V4_0, V4_1, V5_0 };
    enum Errors{ ASL_NO_ERROR, ASL_COMPILATION_FAILED, ASL_LINK_FAILED };
}

MSGPACK_ADD_ENUM(ASL::VarGroup)
MSGPACK_ADD_ENUM(ASL::ShaderModel)
MSGPACK_ADD_ENUM(ASL::Errors)