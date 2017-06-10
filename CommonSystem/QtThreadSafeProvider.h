#pragma once
//
// Created by islam on 18.03.17.
//


#include <MSingleton.h>
#include <QObject>
#include <functional>


class QtThreadSafeProvider: protected QObject, public MSingleton<QtThreadSafeProvider> {
    bool event(QEvent* event) override;
public:
    void execute(std::function<void(void)> action);
};



