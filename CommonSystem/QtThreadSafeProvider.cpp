//
// Created by islam on 18.03.17.
//

#include "QtThreadSafeProvider.h"

#include <QCoreApplication>
#include <QThread>

using namespace std;


class QtThreadSafeProviderEvent: public QEvent {
    std::function<void(void)> action;
public:
    QtThreadSafeProviderEvent(const std::function<void(void)>& action):
        action(action), QEvent(static_cast<QEvent::Type>(QEvent::User + 1))
    {}
    void operator()() {
        action();
    }
};


void QtThreadSafeProvider::execute(function<void(void)> action) {
    if (QThread::currentThread() != this->thread()) {
        qApp->postEvent(this, new QtThreadSafeProviderEvent(action));
    }
    else {
        action();
    }
}

bool QtThreadSafeProvider::event(QEvent *event) {
    auto terminal_event = dynamic_cast<QtThreadSafeProviderEvent*>(event);
    if (terminal_event) {
        (*terminal_event)();
    }
    return QObject::event(event);
}
