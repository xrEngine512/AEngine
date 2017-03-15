#include "AsynchronousThread.h"

#include <Logger.h>
#include <OS.h>

using namespace std;

void AsynchronousThread::Thread()
{
    if(! m_numTasks){
        auto lock = unique_lock<mutex>(m_Mutex);
        cv.wait(lock, [&] { return m_numTasks; });
    }

    add_state(ActivityState::RUNNING);

    while (!m_Shutdown.load())
    {
		UpdateTasks();

        auto from = OS::Time::count_from();
        for (auto it = m_Tasks.begin(); it != m_Tasks.end(); ) {
            if ((*it)() == DONE)
                it = m_Tasks.erase(it);
            else
                ++it;
        }
        m_LoopTime.store(OS::Time::count_to(from));
        this_thread::sleep_for(16ms);
    }

    Logger::instance().debug("Asynchronous worker thread " + get_thread_id() + " is exiting");

    {
        auto lock = unique_lock<mutex>(m_Mutex);
        remove_state(ActivityState::RUNNING);
    }
    cv.notify_one();
}

AsynchronousThread::AsynchronousThread()
{
}


AsynchronousThread::~AsynchronousThread()
{
}
