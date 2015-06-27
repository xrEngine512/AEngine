#include "EventEmitter.h"
#include "IFnPtr.h"

EventEmitter::EventEmitter()
{
}

int EventEmitter::FindHandler(IFnPtr* handler)
{
	int res = 0;
	for (auto h : m_eventHandlers)
	{
		if (handler == h) return res;
		res++;
	}
	return -1;
}

void EventEmitter::operator += (IFnPtr* handler)
{
	if (FindHandler(handler) != -1)
		return;
	m_eventHandlers.push_back(handler);
}

void EventEmitter::operator += (std::function<void(void)> t)
{
	m_lambdaEventHandlers.push_back(t);
}

void EventEmitter::operator -= (IFnPtr* handler)
{
	int index = FindHandler(handler);
	if (index < 0)
		return;
	m_eventHandlers.erase(m_eventHandlers.begin() + index);
}

void EventEmitter::operator()()
{
	for (auto h : m_eventHandlers)
	{
		h->Call();
	}
	for (auto h : m_lambdaEventHandlers)
	{
		h();
	}
}

EventEmitter::~EventEmitter()
{
	for (auto h : m_eventHandlers)
	{
		delete h;
	}
}
