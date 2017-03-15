//
// Created by islam on 08.03.17.
//

#include "Logger.h"

#include <Terminal.h>

using namespace std;
using namespace experimental;


void Logger::out(const string_view &prefix, const string_view &message) {
    time_t t = time(0);
    tm* now = localtime(&t);
    terminal() << prefix << ": " << now->tm_mday << "." << (now->tm_mon + 1) << "." << (now->tm_year + 1900) << " "
                         << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << " >>> " << message << terminal_endl;
    terminal().set_color(TerminalColors::Default);
}

void Logger::log(const string_view &message) {
    terminal().set_color(TerminalColors::Gray);
    out("log", message);
}

void Logger::debug(const string_view &message) {
    terminal().set_color(TerminalColors::White);
    out("debug", message);
}

void Logger::warning(const std_ex::string_view &message) {
    terminal().set_color(TerminalColors::Yellow);
    out("warning", message);
}

void Logger::error(const std_ex::string_view &message) {
    terminal().set_color(TerminalColors::Red);
    out("error", message);
}

Logger &Logger::instance() {
    static Logger instance;
    return instance;
}
