#pragma once
#ifdef SYNC_THREADS
#include "SynchronizedMasterThread.h"
#else
#include "AsynchronousThread.h"
#endif

#include <MState.h>


class APPROX_API SystemThreadPool: MActivityState
{
#ifdef SYNC_THREADS
    vector<SynchronizedSlaveThread*> m_SlaveThreads;
    vector<SynchronizedThread*> threads;
    SynchronizedMasterThread* m_RenderingThread;
#else
	std::vector<AsynchronousThread*> threads;
#endif 
    unsigned int m_numCPU, m_numTasks;
    unsigned int get_optimal_worker_index();
public:
	void add_activity(const std::function<ExitCode(void)> &task, bool isRenderingTask = false);
    void initialize();
    void run();
    void shutdown();
    SystemThreadPool();
    ~SystemThreadPool();
};