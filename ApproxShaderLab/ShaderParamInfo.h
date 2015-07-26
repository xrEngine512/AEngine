#pragma once
#include <string>
#include "AbstractSaveData.h"

namespace ASL
{
	struct ShaderParamInfo : AbstractSaveData
	{
		int ID;
		std::string Type;
		std::string Name;
		const void* Serialize(int& size) override
		{
			size = Serialization(ID, Type, Name);
			return SerializedBuf();
		}
		void Deserialize(const void* buf, size_t size) override
		{
			Deserialization(buf, size, ID, Type, Name);
		}
		int SerializedSize()const override
		{
			return CalculateSize(ID, Type, Name);
		}
		ShaderParamInfo* ToShaderParameterInfo()override
		{
			return this;
		}
	};
}