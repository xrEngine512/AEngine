#include "SystemThreadPool.h"

#include <ApproxSystemErrors.h>
#include <OS.h>

#ifdef SYNC_THREADS
#include "SynchronizedSlaveThread.h"
#endif

SystemThreadPool::SystemThreadPool() :m_numTasks(0)
{
#ifdef SYNC_THREADS
	m_RenderingThread = nullptr;
#endif
}

void SystemThreadPool::initialize()
{
    m_numCPU = OS::Threading::get_core_count();
#ifdef SYNC_THREADS
    m_RenderingThread = new SynchronizedMasterThread;
    threads.push_back(m_RenderingThread);
    for (int i = 0; i < m_numCPU - 1; i++)
    {
        SynchronizedSlaveThread* slave = new SynchronizedSlaveThread(m_RenderingThread->GetOutSync());
        m_RenderingThread->AddSlave(slave);
        threads.push_back(slave);
        m_SlaveThreads.push_back(slave);
    }
#else
	for (int i = 0; i < m_numCPU; i++)
	{
		threads.push_back(new AsynchronousThread);
	}
#endif
    add_state(ActivityState::INITIALIZED);
}

void SystemThreadPool::run()
{
#ifdef SYNC_THREADS
    for (auto thr : m_SlaveThreads)
    {
        thr->Run();
    }
    m_RenderingThread->Run();
#else
	for (auto thr : threads)
	{
		thr->Run();
        break;
	}
#endif
    add_state(ActivityState::RUNNING);
}

unsigned int SystemThreadPool::get_optimal_worker_index()
{
    unsigned int minSizeIndex = 0, index = 0;
	double max_time;
	double time = max_time = threads[0]->GetLoopTime(), cur_time;
    for(auto obj : threads)
    {
		cur_time = obj->GetLoopTime();
		if (cur_time < time) { minSizeIndex = index; time = cur_time; }
		else if (cur_time > max_time) { max_time = cur_time; }
        index++;
    }
	if (max_time == 0)
	{
		index = 0;
		unsigned int min_tasks = threads[0]->GetNumberOfTasks();
		for(auto obj : threads)
		{
			if (obj->GetNumberOfTasks() < min_tasks){ minSizeIndex = index; min_tasks = obj->GetNumberOfTasks(); }
			if (min_tasks == 0) break;
			index++;
		}
	}
    return minSizeIndex;
}

void SystemThreadPool::add_activity(const std::function<ExitCode()> &task, bool isRenderingTask)
{   
#ifdef SYNC_THREADS
    if (isRenderingTask)
    {       
        m_RenderingThread->AddTask(task);
    }
    else
#endif
        threads[get_optimal_worker_index()]->AddTask(task);

    m_numTasks++;
}

void SystemThreadPool::shutdown()
{
#ifdef SYNC_THREADS
    for each (auto wThread in m_SlaveThreads)
    {
        wThread->Shutdown();
        delete wThread;
    }
    delete m_RenderingThread;
#else
	for(auto thr : threads)
	{
        thr->shutdown();
		delete thr;
	}
#endif
    m_numTasks = 0;
    reset_state();
}

SystemThreadPool::~SystemThreadPool()
{
}
