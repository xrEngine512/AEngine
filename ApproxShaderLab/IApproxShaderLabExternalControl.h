#pragma once
#include <vector>
namespace MatInterface
{
	__interface IMaterialVariable;
}
namespace ASL
{
	__interface IApproxShaderLabExternalControl
	{
		void SetMaterialVariables(std::vector<MatInterface::IMaterialVariable*>const& MI_Info);
	};
}