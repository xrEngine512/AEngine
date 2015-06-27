#pragma once
#include "SynchronizedThread.h"

class SynchronizedSlaveThread : public SynchronizedThread
{
    HANDLE m_InSync;

    virtual void Thread()override;
public:
    SynchronizedSlaveThread(HANDLE SyncIn);
    virtual ~SynchronizedSlaveThread();
};

