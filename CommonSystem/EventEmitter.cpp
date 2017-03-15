#include "EventEmitter.h"

using namespace std;

template <typename Fn_Type>
bool functions_equal(const function<Fn_Type>& lhs, const function<Fn_Type>& rhs) {
	return lhs.target_type() == rhs.target_type() &&
		   lhs.template target<Fn_Type>() == rhs.template target<Fn_Type>();
}

EventEmitter::EventEmitter()
{
}

int EventEmitter::FindHandler(const function<void(void)>& handler)
{
	int res = 0;
	for (auto h : m_eventHandlers)
	{
		if (functions_equal(handler, h)) return res;
		res++;
	}
	return -1;
}

void EventEmitter::operator += (const function<void(void)>& handler)
{
	m_eventHandlers.push_back(handler);
}

void EventEmitter::operator -= (const function<void(void)>& handler)
{
	int index = FindHandler(handler);
	if (index < 0)
		return;
	m_eventHandlers.erase(m_eventHandlers.begin() + index);
}

void EventEmitter::operator()()
{
	for (auto handler : m_eventHandlers)
		handler();
}

EventEmitter::~EventEmitter()
{
}
