#include  "ApproxSystemErrors.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

ApproxException::ApproxException(const std::string& msg, Parameter param) :m_Param(param)
{
	copy(msg.begin(), msg.end(), _msg.begin());
}

ApproxException::ApproxException(const std::wstring& msg, Parameter param) : m_Param(param), _msg(msg)
{
}

ApproxException::ApproxException(const std::string& msg, const std::string& className, Parameter param) : m_Param(param)
{
	std::string message = msg + "(In class: " + className + ")";
	copy(message.begin(), message.end(), _msg.begin());
}

ApproxException::ApproxException(const std::wstring& msg, std::wstring className, Parameter param) : m_Param(param), _msg(msg + L"(In class: " + className + L")")
{
}

void ApproxException::ShowMessage(void* hwnd)
{
	MessageBox((HWND)hwnd, _msg.c_str(), L"Initialization error", MB_OK);
}

void ApproxException::operator()(void* hwnd)
{
	if (m_Param == DEFAULT_MSG_DISPLAY)
		ShowMessage(hwnd);
}

const wchar_t * ApproxException::Message()
{
	return _msg.c_str();
}

ApproxException ApproxException::becauseOf(const ApproxException& reason)
{
    ApproxException res(_msg + L" because " + reason._msg);
    return res;
}

void ApproxException::operator+=(ApproxException& reason)
{
    _msg += L" because " + reason._msg;
}