#include  "ApproxSystemErrors.h"
#include <QMessageBox>

using namespace std;
using namespace experimental;

ApproxException::ApproxException(const string_view& msg, Parameter param) :m_Param(param)
{
	_msg = msg.data();
}

ApproxException::ApproxException(const string_view& msg, const string_view& className, Parameter param) : m_Param(param)
{
	_msg = msg.data();
	_msg = _msg + "(In class: " + className.data() + ")";
}

void ApproxException::ShowMessage() const
{
    QMessageBox(QMessageBox::Question, "Title", _msg.c_str(),  QMessageBox::Ok | QMessageBox::Cancel).exec();
}

void ApproxException::operator()()
{
	if (m_Param == DEFAULT_MSG_DISPLAY)
		ShowMessage();
}

const string& ApproxException::Message() const
{
	return _msg;
}

ApproxException ApproxException::becauseOf(const ApproxException& reason)
{
    ApproxException res(_msg + " because " + reason._msg);
    return res;
}

void ApproxException::operator+=(ApproxException& reason)
{
    _msg += " because " + reason._msg;
}