#pragma once
#include <vector>

__interface IMaterialVariable;

__interface IApproxShaderLabExternalControl
{
	void SetSceneProperties(std::vector<IMaterialVariable*>const& MI_Info);
};