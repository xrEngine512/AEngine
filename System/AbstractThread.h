#pragma once

#include <IActivity.h>
#include <MState.h>

#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>


class AbstractThread: public MActivityState
{
protected:
    std::thread *worker_thread;
    std::mutex m_Mutex;
    std::condition_variable cv;
    std::vector<std::function<ExitCode(void)>> m_Tasks;
    std::queue<std::function<ExitCode(void)>> m_newTasks;
    unsigned int   m_numTasks;
    std::atomic_bool m_Shutdown;
    std::atomic<double> m_LoopTime;

    virtual void Thread() = 0;
    void UpdateTasks();
    static std::string get_thread_id();

public:
    AbstractThread();
    void AddTask(const std::function<ExitCode(void)>& task);
    void Run();
    unsigned int GetNumberOfTasks();
    double GetLoopTime();
    void shutdown();
    virtual ~AbstractThread();
};

