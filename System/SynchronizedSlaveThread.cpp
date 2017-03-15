#include "SynchronizedSlaveThread.h"
#define WIN_32_LEAN_AND_MEAN
#include <Windows.h>
#include "SyncEvent.h"

void SynchronizedSlaveThread::Thread()
{
    while (!m_Shutdown.load())
    {
        WaitForSingleObject(m_InSync,INFINITE);
        UpdateTasks();
        if (!m_Tasks.empty())
        {
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
        }
        SetEvent(m_OutSync);
    }
}

SynchronizedSlaveThread::SynchronizedSlaveThread(HANDLE SyncIn) :m_InSync(SyncIn)
{
}


SynchronizedSlaveThread::~SynchronizedSlaveThread()
{
}
