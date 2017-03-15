#pragma once
#include <vector>
namespace MatInterface
{
	struct MaterialInterfaceInfo;
}
namespace ASL
{
	interface IApproxShaderLabExternalControl
	{
		virtual void SetMaterialInterfaceInfo(MatInterface::MaterialInterfaceInfo const& MI_Info) = 0;
		virtual ~IApproxShaderLabExternalControl(){};
	};
}