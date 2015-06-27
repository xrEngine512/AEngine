#pragma once
#include "AbstractThread.h"

class SynchronizedThread : public AbstractThread
{
protected:
    HANDLE m_OutSync;
public:
    SynchronizedThread();
    virtual HANDLE GetOutSync();
    virtual ~SynchronizedThread();
};

