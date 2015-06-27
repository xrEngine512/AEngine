#include "SystemThreadPool.h"
#define WIN_32_LEAN_AND_MEAN
#include <Windows.h>
#include <ApproxSystemErrors.h>
#ifdef SYNC_THREADS
#include "SynchronizedSlaveThread.h"
#endif

SystemThreadPool::SystemThreadPool() :m_numTasks(0)
{
#ifdef SYNC_THREADS
	m_RenderingThread = nullptr;
#endif
}

void SystemThreadPool::Initialize()
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    m_numCPU = sysinfo.dwNumberOfProcessors;
#ifdef SYNC_THREADS
    m_RenderingThread = new SynchronizedMasterThread;
    m_Threads.push_back(m_RenderingThread);
    for (int i = 0; i < m_numCPU - 1; i++)
    {
        SynchronizedSlaveThread* slave = new SynchronizedSlaveThread(m_RenderingThread->GetOutSync());
        m_RenderingThread->AddSlave(slave);
        m_Threads.push_back(slave);
        m_SlaveThreads.push_back(slave);
    }
#else
	for (int i = 0; i < m_numCPU; i++)
	{
		m_Threads.push_back(new AsynchronicThread);
	}
#endif
}

void SystemThreadPool::Run()
{
#ifdef SYNC_THREADS
    for (auto thr : m_SlaveThreads)
    {
        thr->Run();
    }
    m_RenderingThread->Run();
#else
	for (auto thr : m_Threads)
	{
		thr->Run();
	}
#endif
}

unsigned int SystemThreadPool::OptimalWorkerIndex()
{
    unsigned int minSizeIndex = 0, index = 0;
	double max_time;
	double time = max_time = m_Threads[0]->GetLoopTime(), cur_time;
    for each (auto obj in m_Threads)
    {
		cur_time = obj->GetLoopTime();
		if (cur_time < time) { minSizeIndex = index; time = cur_time; }
		else if (cur_time > max_time) { max_time = cur_time; }
        index++;
    }
	if (max_time == 0)
	{
		index = 0;
		unsigned int min_tasks = m_Threads[0]->GetNumberOfTasks();
		for each (auto obj in m_Threads)
		{
			if (obj->GetNumberOfTasks() < min_tasks){ minSizeIndex = index; min_tasks = obj->GetNumberOfTasks(); }
			if (min_tasks == 0) break;
			index++;
		}
	}
    return minSizeIndex;
}

void SystemThreadPool::AddTask(IFnPtr* task, bool isRenderingTask)
{   
#ifdef SYNC_THREADS
    if (isRenderingTask)
    {       
        m_RenderingThread->AddTask(task);
    }
    else
#endif
        m_Threads[OptimalWorkerIndex()]->AddTask(task);

    m_numTasks++;
}

void SystemThreadPool::Shutdown()
{
#ifdef SYNC_THREADS
    for each (auto wThread in m_SlaveThreads)
    {
        wThread->Shutdown();
        delete wThread;
    }
    delete m_RenderingThread;
#else
	for(auto thr : m_Threads)
	{
		thr->Shutdown();
		delete thr;
	}
#endif
    m_numTasks = 0;
}

SystemThreadPool::~SystemThreadPool()
{
}
