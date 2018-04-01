#pragma once
#include "Enums.h"
#include "serialization/RuntimeBufferInfo.h"
#include "serialization/ShaderCommon.h"

namespace ASL
{
	struct ShaderPart
	{
		std::string Str_code;
		ShaderType shader_type = NONE;
		std::string EntryPoint;
		vector<RuntimeBufferInfo> BuffersInfo;
		vector<int> ParamsIDs;
		vector<int> TextureSlots;

		char CurrentSymbol()const{ return Str_code[pos]; }
		void PosInc(){ pos++; }
		int pos = 0;
		bool PosInRange() const{ return pos < Str_code.size(); }

		MSGPACK_DEFINE(Str_code, shader_type, EntryPoint, BuffersInfo, ParamsIDs, TextureSlots);
	};
}
