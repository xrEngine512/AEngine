#pragma once
#include <vector>
#include "IMaterialVariable.h"
#include "AbstractSaveData.h"

namespace ASL
{
	struct RuntimeBufferInfo : AbstractSaveData
	{
		MatInterface::VarGroup group;
		std::vector<int> IDs;
		
		const void* Serialize(int& size)override
		{
			size = Serialization(group, IDs);
			return SerializedBuf();
		}
		void Deserialize(const void* buf, size_t size)override
		{
			Deserialization(buf, size, group, IDs);
		}
		int SerializedSize()const override
		{
			return CalculateSize(group, IDs);
		}
		RuntimeBufferInfo* ToRuntimeBufferInfo()override
		{
			return this;
		}

	};
}
