#pragma once
#include <vector>
#include <functional>
__interface IFnPtr;

class EventEmitter
{
	std::vector<IFnPtr*> m_eventHandlers;
	std::vector<std::function<void(void)>> m_lambdaEventHandlers;
	int FindHandler(IFnPtr* handler);
public:
	EventEmitter();
	void operator+=(IFnPtr* handler);
	void operator+=(std::function<void(void)>);
	void operator-=(IFnPtr* handler);
	void operator()();
	~EventEmitter();
};