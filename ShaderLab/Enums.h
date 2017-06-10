#pragma once

namespace ASL
{
    enum Shader_Type : char { ST_NONE ,VS, PS, GS, CS, DS, HS };
	enum VarGroup : char { SCENE_CONSTANT, SHADER_PARAMS, PER_FRAME, PER_OBJECT };
    enum Shader_Model{ SM_NONE, V4_0, V4_1, V5_0 };
    enum Errors{ ASL_NO_ERROR, ASL_COMPILATION_FAILED, ASL_LINK_FAILED };
}