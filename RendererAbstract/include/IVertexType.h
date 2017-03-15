#pragma once

enum VertexTypes 
{
	VT_DEFAULT, VT_PC, VT_PT, VT_PTN, VT_PCT, VT_PCTN
};

interface IVertexType
{
	virtual int GetSize() = 0;
	virtual VertexTypes GetType() = 0;
    virtual ~IVertexType(){};
};