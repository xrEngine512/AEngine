#pragma once
#include "AbstractThread.h"

class AsynchronousThread :
    public AbstractThread
{
    void Thread()override;
public:
    AsynchronousThread();
    virtual ~AsynchronousThread();
};

