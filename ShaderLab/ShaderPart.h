#pragma once
#include "Enums.h"
#include "RuntimeBufferInfo.h"

namespace ASL
{
	using std::string;
	struct ShaderPart : AbstractSaveData
	{
		string Str_code;
		Shader_Type shader_type = ST_NONE;
		string EntryPoint;
		vector<RuntimeBufferInfo> BuffersInfo;
		vector<int> ParamsIDs;
		vector<int> TextureSlots;

		char CurrentSymbol()const{ return Str_code[pos]; }
		void PosInc(){ pos++; }
		int pos = 0;
		bool PosInRange() const{ return pos < Str_code.size(); }
		
		const void* Serialize(int& size)override final
		{
			size = Serialization(Str_code, shader_type, EntryPoint, BuffersInfo, ParamsIDs, TextureSlots);
			return SerializedBuf();
		}
		void Deserialize(const void* buf, size_t size)override final
		{
			Deserialization(buf, size, Str_code, shader_type, EntryPoint, BuffersInfo, ParamsIDs, TextureSlots);
		}
		int SerializedSize()const override final
		{
			return CalculateSize(Str_code, shader_type, EntryPoint, BuffersInfo, ParamsIDs, TextureSlots);
		}
	};
}
