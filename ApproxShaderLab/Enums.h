#pragma once

namespace ASL
{
    enum Shader_Type : __int8 { ST_NONE ,VS, PS, GS, CS, DS, HS };
	enum VarGroup : __int8 { SCENE_CONSTANT, SHADER_PARAMS, PER_FRAME, PER_OBJECT };
    enum Shader_Model{ SM_NONE, V4_0, V4_1, V5_0 };
    enum Errors{ ASL_NO_ERROR, ASL_COMPILATION_FAILED, ASL_NO_VSI_NAME };
}