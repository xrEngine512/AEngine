#pragma once
#include <vector>
namespace MatInterface
{
	struct MaterialInterfaceInfo;
}
namespace ASL
{
	__interface IApproxShaderLabExternalControl
	{
		void SetMaterialInterfaceInfo(MatInterface::MaterialInterfaceInfo const& MI_Info);
	};
}