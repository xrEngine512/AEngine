#include  "ApproxSystemErrors.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

ApproxException::ApproxException(wchar_t* msg, Parameter param) :_msg(msg), m_Param(param)
{
	
}

ApproxException::ApproxException(std::wstring msg, Parameter param) : _msg(msg), m_Param(param)
{
}

ApproxException::ApproxException(wchar_t* msg, wchar_t* className, Parameter param) : m_Param(param)
{
    _msg = std::wstring(msg) + L"(In class: " + std::wstring(className) + L")";
}

ApproxException::ApproxException(std::wstring msg, std::wstring className, Parameter param) : _msg(msg + L"(In class: " + className + L")"), m_Param(param)
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

ApproxException ApproxException::becauseOf(ApproxException& reason)
{
    ApproxException res(_msg + L" because " + reason._msg);
    return res;
}

void ApproxException::operator+=(ApproxException& reason)
{
    _msg += L" because " + reason._msg;
}