#pragma once
#include "AbstractThread.h"
class AsynchronicThread :
    public AbstractThread
{
    void Thread()override;
public:
    AsynchronicThread();
    virtual ~AsynchronicThread();
};

