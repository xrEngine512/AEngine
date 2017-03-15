#pragma once
#include <string>
#include <experimental/string_view>

class ApproxException
{
public:
	enum Parameter{ DEFAULT_MSG_DISPLAY, INSTANT_MSG_DISPLAY };
private:

	Parameter m_Param = DEFAULT_MSG_DISPLAY;
	std::string _msg;

	void ShowMessage() const;
public:
	explicit ApproxException(const std::experimental::string_view& msg, Parameter param = DEFAULT_MSG_DISPLAY);
	ApproxException(const std::experimental::string_view& msg, const std::experimental::string_view& className, Parameter param = DEFAULT_MSG_DISPLAY);
    void operator()();
	ApproxException becauseOf(const ApproxException& reason);
	void operator+=(ApproxException& reason);
	const std::string& Message() const;
};