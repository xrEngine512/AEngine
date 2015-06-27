#pragma once
#ifdef SYNC_THREADS
#include "SynchronizedMasterThread.h"
#else
#include "AsynchronicThread.h"
#endif 

using namespace std;
class __declspec(dllexport) SystemThreadPool
{
#ifdef SYNC_THREADS
    vector<SynchronizedSlaveThread*> m_SlaveThreads;
    vector<SynchronizedThread*> m_Threads;
    SynchronizedMasterThread* m_RenderingThread;
#else
	vector<AsynchronicThread*> m_Threads;
#endif 
    unsigned int m_numCPU, m_numTasks;
    unsigned int OptimalWorkerIndex();
public:
    void AddTask(IFnPtr* task, bool isRenderingTask = false);
    void Initialize();
    void Run();
    void Shutdown();
    SystemThreadPool();
    ~SystemThreadPool();
};