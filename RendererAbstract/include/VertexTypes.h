#pragma once

#include "IVertexType.h"

#include <ApproxMath.h>

struct PosTexNormVert;
struct PosTexVert;

typedef uint32_t index_t;

struct DefaultVertex : public IVertexType
{
	DefaultVertex(){};
    DefaultVertex(float, float, float, float w = 0);
    DefaultVertex(const Math::AVector& position, const Math::AVector2& texture);
	DefaultVertex(const DefaultVertex& vertex);

	struct Data {
		Math::AVector position = Math::AVector();
		Math::AVector2 texture = Math::AVector2();
		Math::AVector3 normal = Math::AVector3();
	} data;

	bool operator==(const DefaultVertex& lhr);
	bool operator!=(const DefaultVertex& lhr);
	int GetSize();
	VertexTypes GetType();
};

struct PosColorVert : public IVertexType
{
    PosColorVert(){};
	PosColorVert(PosTexVert& parent);
	PosColorVert(PosTexNormVert& parent);
	Math::AVector3 position;
	Math::AVector color;

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
	Math::AVector3 position;
	Math::AVector2 texture;
};

struct PosTexVert : public IVertexType
{
    PosTexVert(){};
	PosTexVert(PosColorVert& parent);
	PosTexVert(PosTexNormVert& parent);

	Math::AVector3 position;
	Math::AVector2 texture;

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
	Math::AVector3 position;
	Math::AVector2 texture;
	Math::AVector3 normal;
	int GetSize()
	{
		return sizeof(PosTexNormVert);
	}
	VertexTypes GetType()
	{
		return VT_PTN;
	}
};