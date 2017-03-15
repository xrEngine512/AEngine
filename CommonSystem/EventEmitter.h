#pragma once
#include <vector>
#include <functional>


class EventEmitter
{
	std::vector<std::function<void(void)>> m_eventHandlers;
	int FindHandler(const std::function<void(void)>& handler);
public:
	EventEmitter();
	void operator+=(const std::function<void(void)>&);
	void operator-=(const std::function<void(void)>&);
	void operator()();
	~EventEmitter();
};