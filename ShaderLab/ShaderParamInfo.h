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
		const void* Serialize(uint64_t& size) override
		{
			size = Serialization(ID, Type, Name);
			return SerializedBuf();
		}
		void Deserialize(const void* buf, uint64_t size) override
		{
			Deserialization(buf, size, ID, Type, Name);
		}
		uint64_t SerializedSize()const override
		{
			return CalculateSize(ID, Type, Name);
		}
	};
}