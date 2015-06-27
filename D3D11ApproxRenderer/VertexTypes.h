#pragma once

#include "IVertexType.h"

struct PosTexNormVert;
struct PosTexVert;

struct PosColorVert : public IVertexType
{
    PosColorVert(){};
	PosColorVert(PosTexVert& parent);
	PosColorVert(PosTexNormVert& parent);
	XMFLOAT3 position;
	XMFLOAT4 color;

	int GetSize()
	{
		return sizeof(PosColorVert);
	}
	VertexTypes GetType()
	{
		return VT_PC;
	}
};

struct PosTexVertStruct
{
    XMFLOAT3 position;
    XMFLOAT2 texture;
};

struct PosTexVert : public IVertexType
{
    PosTexVert(){};
	PosTexVert(PosColorVert& parent);
	PosTexVert(PosTexNormVert& parent);
    
    XMFLOAT3 position;
    XMFLOAT2 texture;

	int GetSize()
	{
		return sizeof(PosTexVert);
	}
	VertexTypes GetType()
	{
		return VT_PT;
	}
};

struct PosTexNormVert : public IVertexType
{
    PosTexNormVert(){};
	PosTexNormVert(PosColorVert& parent);
	PosTexNormVert(PosTexVert& parent);
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
	int GetSize()
	{
		return sizeof(PosTexNormVert);
	}
	VertexTypes GetType()
	{
		return VT_PTN;
	}
};