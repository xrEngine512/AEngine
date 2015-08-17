#pragma once
#include <string>

class ApproxException
{
public:
	enum Parameter{ DEFAULT_MSG_DISPLAY, INSTANT_MSG_DISPLAY };
private:

	Parameter m_Param = DEFAULT_MSG_DISPLAY;
	std::wstring _msg;

	void ShowMessage(void* hwnd);
public:
	explicit ApproxException(const std::wstring& msg, Parameter param = DEFAULT_MSG_DISPLAY);
	explicit ApproxException(const std::string& msg, Parameter param = DEFAULT_MSG_DISPLAY);
	ApproxException(const std::string& msg, const std::string& className, Parameter param = DEFAULT_MSG_DISPLAY);
	ApproxException(const std::wstring& msg, std::wstring className, Parameter param = DEFAULT_MSG_DISPLAY);
    void operator()(void* hwnd);
	ApproxException becauseOf(const ApproxException& reason);
	void operator+=(ApproxException& reason);
	const wchar_t * Message();
};