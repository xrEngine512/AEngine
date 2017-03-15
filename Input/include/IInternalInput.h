#pragma once

#include <IActivity.h>

interface IInternalInput: IActivity
{
    virtual void Initialize() = 0;
    virtual void SetWndCenter(int x, int y) = 0;
    virtual void onKeyDown(int key) = 0;
    virtual void onKeyUp(int key) = 0;
    virtual void onMouseDown(int key) = 0;
    virtual void onMouseUp(int key) = 0;
    virtual ~IInternalInput(){};
};

IInternalInput* GetInput();
