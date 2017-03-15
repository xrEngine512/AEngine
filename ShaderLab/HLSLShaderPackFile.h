#pragma once

#include <d3d11.h>


using std::vector;

struct D3D11_INPUT_LAYOUT_ELEMENT
{
	UINT SemanticIndex;
	DXGI_FORMAT Format;
	UINT InputSlot;
	UINT AlignedByteOffset;
	D3D11_INPUT_CLASSIFICATION InputSlotClass;
	UINT InstanceDataStepRate;
	D3D11_INPUT_ELEMENT_DESC ToElemDesc()
	{
		D3D11_INPUT_ELEMENT_DESC res;
		res.AlignedByteOffset = AlignedByteOffset;
		res.Format = Format;
		res.InputSlot = InputSlot;
		res.InputSlotClass = InputSlotClass;
		res.InstanceDataStepRate = InstanceDataStepRate;
		res.SemanticIndex = SemanticIndex;
		res.SemanticName = nullptr;
		return res;
	}
};
