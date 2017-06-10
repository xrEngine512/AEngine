#pragma once
#include <string>
#include "AbstractSaveData.h"

namespace ASL
{
	struct TextureInfo : AbstractSaveData
	{
		uint8_t Slot;
		std::string Name;
		const void* Serialize(uint64_t& size) override
		{
			size = Serialization(Slot, Name);
			return SerializedBuf();
		}
		void Deserialize(const void* buf, uint64_t size)override
		{
			Deserialization(buf,size, Slot, Name);
		}
		uint64_t SerializedSize()const override
		{
			return CalculateSize(Slot, Name);
		}
	};
}