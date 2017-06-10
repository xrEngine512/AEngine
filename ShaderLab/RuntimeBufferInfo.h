#pragma once
#include "AbstractSaveData.h"

#include <vector>
#include <IMaterialVariable.h>

namespace ASL
{
	struct RuntimeBufferInfo : AbstractSaveData
	{
		MatInterface::VarGroup group;
		std::vector<int> IDs;
		
		const void* Serialize(uint64_t & size)override
		{
			size = Serialization(group, IDs);
			return SerializedBuf();
		}
		void Deserialize(const void* buf, uint64_t size)override
		{
			Deserialization(buf, size, group, IDs);
		}
		uint64_t SerializedSize()const override
		{
			return CalculateSize(group, IDs);
		}
	};
}
