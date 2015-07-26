#pragma once
#include "Enums.h"
#include "RuntimeBufferInfo.h"

namespace ASL
{
	using std::string;
	struct ShaderPart : AbstractSaveData
	{
		string Str_code;
		Shader_Type Shader_Type = ST_NONE;
		string EntryPoint;
		vector<RuntimeBufferInfo> BuffersInfo;
		vector<int> ParamsIDs;

		char CurrentSymbol()const{ return Str_code[pos]; }
		void PosInc(){ pos++; }
		int pos = 0;
		bool PosInRange() const{ return pos < Str_code.size(); }
		
		const void* Serialize(int& size)override
		{
			size = Serialization(Str_code, Shader_Type, EntryPoint, BuffersInfo, ParamsIDs);
			return SerializedBuf();
		}
		void Deserialize(const void* buf, size_t size)override
		{
			Deserialization(buf, size, Str_code, Shader_Type, EntryPoint, BuffersInfo, ParamsIDs);
		}
		int SerializedSize()const override
		{
			return CalculateSize(Str_code, Shader_Type, EntryPoint, BuffersInfo, ParamsIDs);
		}
	};
}
