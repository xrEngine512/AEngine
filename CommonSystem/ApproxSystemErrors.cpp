#include "ApproxSystemErrors.h"

#include <Logger.h>
#include <QtThreadSafeProvider.h>
#include <QMessageBox>

using namespace std;
using namespace experimental;

approx_exception::approx_exception(const string_view& msg, Parameter param) :m_Param(param)
{
	_msg = msg.data();
}

approx_exception::approx_exception(const string_view& msg, const string_view& className, Parameter param) : m_Param(param)
{
	_msg = msg.data();
	_msg = _msg + "(In class: " + className.data() + ")";
}

void approx_exception::ShowMessage() const
{
	Logger::instance().error(_msg);
    QtThreadSafeProvider::instance().execute([message = _msg] {
        QMessageBox(QMessageBox::Question, "ApproxEngine::Error", message.c_str(),  QMessageBox::Ok | QMessageBox::Cancel).exec();
    });
}

void approx_exception::operator()()
{
	if (m_Param == DEFAULT_MSG_DISPLAY)
		ShowMessage();
}

const string& approx_exception::Message() const
{
	return _msg;
}

approx_exception approx_exception::becauseOf(const approx_exception& reason)
{
    approx_exception res(_msg + " because " + reason._msg);
    return res;
}

void approx_exception::operator+=(const approx_exception& reason)
{
    _msg += " because " + reason._msg;
}

const char *approx_exception::what() const noexcept {
	return _msg.c_str();
}
