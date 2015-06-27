#include "AbstractThread.h"
#define WIN_32_LEAN_AND_MEAN
#include <Windows.h>

void AbstractThread::UpdateTasks()
{
    if (m_Mutex.try_lock())
    {
        while (!m_newTasks.empty())
        {
            m_Tasks.push_back(m_newTasks.front());
            m_newTasks.pop();
        }
        m_Mutex.unlock();
    }
}

AbstractThread::AbstractThread() :m_WorkerThread(nullptr), m_numTasks(0), m_LoopTime(0)
{
    
}

void AbstractThread::AddTask(IFnPtr* task)
{
    m_Mutex.lock();
	++m_numTasks;
        m_newTasks.push(task);

    m_Mutex.unlock();
}

void AbstractThread::Run()
{
    m_Shutdown.store(false);
    m_WorkerThread = new thread(&AbstractThread::Thread, this);
}

unsigned AbstractThread::GetNumberOfTasks()
{
    return m_numTasks;
}

double AbstractThread::GetLoopTime()
{
    return m_LoopTime;
}

void AbstractThread::Shutdown()
{
    m_Shutdown.store(true);
    Sleep(100);
    TerminateThread(m_WorkerThread->native_handle(), 2);
    
    if (m_WorkerThread)
    {
        if (m_WorkerThread->joinable())
            m_WorkerThread->join();
        delete m_WorkerThread;
        m_WorkerThread = nullptr;
    }
    for (IFnPtr* task : m_Tasks)
    {
        delete task;
        task = nullptr;
    }
    m_numTasks = 0;
    while (!m_newTasks.empty())
    {
        delete m_newTasks.front();
        m_newTasks.pop();
    }
}

AbstractThread::~AbstractThread()
{
}
