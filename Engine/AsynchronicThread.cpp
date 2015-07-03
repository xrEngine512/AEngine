#include "AsynchronicThread.h"
#define WIN_32_LEAN_AND_MEAN
#include <Windows.h>

void AsynchronicThread::Thread()
{
    while (!m_Shutdown.load())
    {
		UpdateTasks();
		
        UpdateTasks();

        LARGE_INTEGER frequency;
        LARGE_INTEGER t1, t2;
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
		QueryPerformanceFrequency(&frequency);
        m_LoopTime.store((t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart);
    }
}

AsynchronicThread::AsynchronicThread()
{
}


AsynchronicThread::~AsynchronicThread()
{
}
