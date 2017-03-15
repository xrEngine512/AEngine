#include "SynchronizedThread.h"
#define WIN_32_LEAN_AND_MEAN
#include <Windows.h>

SynchronizedThread::SynchronizedThread()
{
    m_OutSync = CreateEvent(NULL, NULL, 0, NULL);
}

HANDLE SynchronizedThread::GetOutSync()
{
    return m_OutSync;
}

SynchronizedThread::~SynchronizedThread()
{
    CloseHandle(m_OutSync);
}
