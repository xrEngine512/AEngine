#pragma once

enum VertexTypes 
{
	VT_PC, VT_PT, VT_PTN,VT_PCT, VT_PCTN
};

__interface IVertexType
{
	int GetSize();
	VertexTypes GetType();
};