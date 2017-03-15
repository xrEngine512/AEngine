#pragma once
//
// Created by islam on 08.03.17.
//

#include <experimental/string_view>

namespace std_ex = std::experimental;

class Logger {
    void out(const std_ex::string_view& prefix, const std_ex::string_view& message);
public:
    void log(const std_ex::string_view& message);
    void debug(const std_ex::string_view& message);
    void warning(const std_ex::string_view& message);
    void error(const std_ex::string_view& message);
    static Logger& instance();
};



