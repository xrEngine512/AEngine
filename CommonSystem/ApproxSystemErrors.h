#pragma once

#include <exception>
#include <string>
#include <experimental/string_view>

class approx_exception: public std::exception
{
public:
	enum Parameter{ DEFAULT_MSG_DISPLAY, INSTANT_MSG_DISPLAY };
private:

	Parameter m_Param = DEFAULT_MSG_DISPLAY;
	std::string _msg;

	void ShowMessage() const;
public:
	explicit approx_exception(const std::experimental::string_view& msg, Parameter param = DEFAULT_MSG_DISPLAY);
	approx_exception(const std::experimental::string_view& msg, const std::experimental::string_view& className, Parameter param = DEFAULT_MSG_DISPLAY);
    void operator()();
	approx_exception becauseOf(const approx_exception& reason);
	void operator+=(const approx_exception& reason);

	const char* what() const noexcept override;

	const std::string& Message() const;
};