#include "AbstractThread.h"
#include <OS.h>
#include <Logger.h>

using namespace std;

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

AbstractThread::AbstractThread() :worker_thread(nullptr), m_numTasks(0), m_LoopTime(0)
{
    add_state(ActivityState::INITIALIZED);
}

void AbstractThread::AddTask(const std::function<ExitCode(void)>& task)
{
    if (!m_numTasks) {
        if (!worker_thread) {
            m_Tasks.push_back(task);
            ++m_numTasks;
        }
        else {
            {
                auto lock = unique_lock<mutex>(m_Mutex);
                ++m_numTasks;
                m_newTasks.push(task);
            }
            cv.notify_one();
        }
    } else {
        auto lock = unique_lock<mutex>(m_Mutex);
        ++m_numTasks;
        m_newTasks.push(task);
    }
}

void AbstractThread::Run()
{
    m_Shutdown.store(false);
    worker_thread = new thread(&AbstractThread::Thread, this);
}

unsigned AbstractThread::GetNumberOfTasks()
{
    return m_numTasks;
}

double AbstractThread::GetLoopTime()
{
    return m_LoopTime;
}

void AbstractThread::shutdown()
{
    m_Shutdown.store(true);
    {
        auto lock = unique_lock<mutex>(m_Mutex);
        if (!cv.wait_for(lock, 100ms, [&]{return !has_state(ActivityState::RUNNING);})) {
            Logger::instance().warning("timeout during worker thread " + get_thread_id() + " shutdown, terminating thread...");
            OS::Threading::terminate_thread(*worker_thread);
        }
    }

    if (worker_thread)
    {
        if (worker_thread->joinable())
            worker_thread->join();

        delete worker_thread;
        worker_thread = nullptr;
    }

    m_Tasks.clear();

    m_numTasks = 0;
    while (!m_newTasks.empty())
    {
        m_newTasks.pop();
    }
}

AbstractThread::~AbstractThread()
{
}

std::string AbstractThread::get_thread_id() {
    return to_string(hash<thread::id>()(this_thread::get_id()));
}
