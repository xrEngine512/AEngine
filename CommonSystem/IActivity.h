#pragma once
//
// Created by islam on 12.03.17.
//


#include <functional>


enum ExitCode { DONE, RUNNING, SUSPENDED };

typedef std::function<ExitCode(void)> Activity;

interface IActivity {
    virtual Activity get_activity() = 0;
};



