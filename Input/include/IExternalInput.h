#pragma once

class EventEmitter;

interface IExternalInput
{
	virtual bool IsKeyDown(int) = 0;
	virtual EventEmitter& BindToKey(int key) = 0;
	virtual bool GetMouseCoord(float &x, float &y) = 0;
	virtual bool GetMouseDelta(int &x, int &y) = 0;
	virtual bool IsIncrementalMouseControl() = 0;
	virtual ~IExternalInput(){};
};