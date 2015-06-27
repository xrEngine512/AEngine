#pragma once
#include "Enums.h"

namespace ASL
{
	struct ShaderPart
	{
		typedef std::string strType;
		strType Str_code;
		Shader_Type Shader_Type = ST_NONE;
		strType EntryPoint;
		char CurrentSymbol()const{ return Str_code[pos]; }
		void PosInc(){ pos++; }
		int pos = 0;
		bool PosInRange() const{ return pos < Str_code.size(); }
	};
}
