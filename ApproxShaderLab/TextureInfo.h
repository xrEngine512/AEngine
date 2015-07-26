#pragma once
#include <string>
#include "AbstractSaveData.h"

namespace ASL
{
	struct TextureInfo : AbstractSaveData
	{
		int Slot;
		std::string Name;
		const void* Serialize(int& size) override
		{
			size = Serialization(Slot, Name);
			return SerializedBuf();
		}
		void Deserialize(const void* buf, size_t size)override
		{
			Deserialization(buf,size, Slot, Name);
		}
		int SerializedSize()const override
		{
			return CalculateSize(Slot, Name);
		}
		TextureInfo* ToTextureInfo()override
		{
			return this;
		}
	};
}