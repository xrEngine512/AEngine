#pragma once
#include <vector>
#include <string>
#include "GenericType.h"

namespace ASL
{
	struct RuntimeBufferInfo;
	struct TextureInfo;
	struct ShaderParameterInfo;
}

namespace ShaderSystem
{
	using std::vector;
	using std::string;

	union floatVariant
	{
		float float1;
		float float2[2];
		float float3[3];
		float float4[4];
	};

	struct ShaderSettings{
		vector<ASL::TextureInfo> TexturesInfo;
		vector<ASL::ShaderParameterInfo> ParamsInfo;
	};

	inline GenericType ReadType(const string& type)
	{
		if (type.compare("float") == 0)
		{
			return FLOAT;
		}
		if (type.compare("float4") == 0)
		{
			return FLOAT4;
		}
		if (type.compare("float3") == 0)
		{
			return FLOAT3;
		}
		if (type.compare("float2") == 0)
		{
			return FLOAT2;
		}
		if (type.compare("matrix") == 0)
		{
			return MATRIX;
		}
		return NONE;
	}
}