#pragma once
//
// Created by islam on 11.03.17.
//

#include <QTextEdit>

#include <experimental/string_view>


namespace TerminalColors {
    const auto Red          = "#FF0000";
    const auto Yellow       = "#F4D742";
    const auto Default      = "#FFFFFF";
    const auto White        = "#FFFFFF";
    const auto Gray         = "#888888";
}

const auto terminal_endl = "\n";
const auto terminal_tab  = "&emsp;";

class Terminal: private QTextEdit {
    Q_OBJECT

    Terminal(QWidget* parent = nullptr);

    std::string     html_color      = TerminalColors::Default;
    std::string     stored          = "";
    bool            opened_tag      = false;
    int             stdout_color    = -1;

    void open_tag();
    void close_tag();
    void stdout_colored(const std::string&);
    void insert_html_at_end(const std::string& html);
public:
    void set_color(const std::string& html_color);

    Terminal& operator<<(const char*);
    Terminal& operator<<(char*);
    Terminal& operator<<(const std::string&);
    Terminal& operator<<(const std::experimental::string_view&);

    template <typename T>
    Terminal &operator<<(T value) {
        operator<<(std::to_string(value));
        return *this;
    }

    static Terminal& instance();

    ~Terminal();
};

// Just a shortcut
inline Terminal& terminal() {
    return Terminal::instance();
}

