#pragma once

class EventEmitter;

__interface IExternalInput
{
    bool IsKeyDown(unsigned int);
	EventEmitter& BindToKey(UINT key);
    bool GetMouseCoord(float &x, float &y);
    bool GetMouseDelta(int &x, int &y);
    bool IsIncrementalMouseControl();
};