#pragma once

#include "SynchronizedThread.h"

class SynchronizedSlaveThread;

class SynchronizedMasterThread : public SynchronizedThread
{    
    vector<HANDLE> m_InSyncs, m_OutSyncs;
    void Thread()override;
    void Slave_Thread();    

public:
    void AddSlave(SynchronizedSlaveThread*);
    HANDLE GetOutSync()override;

    
    SynchronizedMasterThread();
    virtual ~SynchronizedMasterThread();
};

