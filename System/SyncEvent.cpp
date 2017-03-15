#include "SyncEvent.h"


SyncEvent::SyncEvent()
{
    m_Event = CreateEventEx(NULL, NULL, 0, SYNCHRONIZE);
}

void SyncEvent::Wait()
{
    WaitForSingleObject(m_Event, INFINITE);
}

void SyncEvent::Notify()
{
    SetEvent(m_Event);
}

SyncEvent::~SyncEvent()
{
}
