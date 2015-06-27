#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <mutex>
#include <IFnPtr.h>

using namespace std;

class AbstractThread
{
protected:
    thread *m_WorkerThread;
    mutex m_Mutex;
    vector<IFnPtr*> m_Tasks;
    queue<IFnPtr*> m_newTasks;
    unsigned int   m_numTasks;
    atomic_bool m_Shutdown;    
    atomic<double> m_LoopTime;
    virtual void Thread() = 0;
    void UpdateTasks();
public:
    AbstractThread();
    void AddTask(IFnPtr* task);
    void Run();
    unsigned int GetNumberOfTasks();
    double GetLoopTime();
    void Shutdown();
    virtual ~AbstractThread();
};

