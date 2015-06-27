#include "SynchronizedMasterThread.h"
#define WIN_32_LEAN_AND_MEAN
#include <Windows.h>
#include <ApproxSystemErrors.h>
#include "SynchronizedSlaveThread.h"
#include "SyncEvent.h"

SynchronizedMasterThread::SynchronizedMasterThread()
{    
    //m_OutSyncs.push_back(m_OutSync);
}

void SynchronizedMasterThread::Thread()
{
    while (!m_Shutdown.load())
    {
        for (auto lock : m_OutSyncs)        //Оповещение всех подчиненных потоков о начале такта кадра
        {
            SetEvent(lock);
        }

        UpdateTasks();

        LARGE_INTEGER frequency;
        LARGE_INTEGER t1, t2;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&t1);
        for (auto threadFunc = m_Tasks.begin(); threadFunc < m_Tasks.end();)
        {
            if ((*threadFunc)->Call() == DONE)
            {
                delete *threadFunc;
                threadFunc = m_Tasks.erase(threadFunc);
            }
            else
            {
                ++threadFunc;
            }
        }
        QueryPerformanceCounter(&t2);
        m_LoopTime.store((t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart);


        for (auto inLock : m_InSyncs)
        {
            WaitForSingleObject(inLock,INFINITE);
        }
    }
}

void SynchronizedMasterThread::AddSlave(SynchronizedSlaveThread* slave)
{
    m_InSyncs.push_back(slave->GetOutSync());
}

HANDLE SynchronizedMasterThread::GetOutSync()
{
    HANDLE h = CreateEvent(NULL, NULL, 0, NULL);
    m_OutSyncs.push_back(h);
    return h;
}

void SynchronizedMasterThread::Slave_Thread()
{
    
}

SynchronizedMasterThread::~SynchronizedMasterThread()
{
}
