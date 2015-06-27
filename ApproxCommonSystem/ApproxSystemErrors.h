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
	ApproxException(wchar_t* msg, Parameter param = DEFAULT_MSG_DISPLAY);
	explicit ApproxException(std::wstring msg, Parameter param = DEFAULT_MSG_DISPLAY);
	ApproxException(wchar_t* msg, wchar_t* className, Parameter param = DEFAULT_MSG_DISPLAY);
	ApproxException(std::wstring msg, std::wstring className, Parameter param = DEFAULT_MSG_DISPLAY);
    void operator()(void* hwnd);
	ApproxException becauseOf(ApproxException& reason);
	void operator+=(ApproxException& reason);
	const wchar_t * Message();
};