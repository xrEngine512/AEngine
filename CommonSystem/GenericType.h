#pragma once

namespace ShaderSystem
{
	enum GenericType
	{
		NONE = 0,
		FLOAT = sizeof(float),
		FLOAT2 = sizeof(float) * 2,
		FLOAT3 = sizeof(float) * 3,
		FLOAT4 = sizeof(float) * 4,
		MATRIX = sizeof(float) * 4 * 4
	};
}