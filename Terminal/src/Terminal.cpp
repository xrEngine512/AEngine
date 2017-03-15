//
// Created by islam on 11.03.17.
//

#include "Terminal.h"

#include <StringUtils.h>
#include <rlutil.h>

#include <atomic>
#include <mutex>

using namespace std;

static Terminal* terminal_instance = nullptr;


Terminal::Terminal(QWidget * parent) : QTextEdit(parent){
    resize(600, 400);
    setFont(QFont("Consolas"));
    setReadOnly(true);
    setLineWrapMode(NoWrap);
    setWindowTitle("ApproxEngine Debug console");
}

Terminal &Terminal::operator<<(const char * text) {
    operator<<(string(text));
    return *this;
}

Terminal& Terminal::operator<<(const string & text) {
    if (text == terminal_endl) {
        if (opened_tag)
            close_tag();
        insertPlainText(terminal_endl);
    }
    else {
        if (!opened_tag)
            open_tag();
        stored += replace_substrings(text, ">", "&gt;");
    }
    stdout_colored(text);

    return *this;
}

Terminal& Terminal::operator<<(const std::experimental::string_view & text) {
    operator<<(string(text.data()));
    return *this;
}

Terminal &Terminal::instance() {
    static mutex mtx;

    auto lock = unique_lock<mutex>(mtx);

    if(!terminal_instance ) {
        terminal_instance = new Terminal();
        terminal_instance->show();
    }

    return *terminal_instance ;
}

Terminal &Terminal::operator<<(char * text) {
    operator<<(string(text));
    return *this;
}

void Terminal::set_color(const std::string &html_color) {
    if (html_color != this->html_color) {
        this->html_color = html_color;
        if (opened_tag)
            close_tag();
    }

    if(html_color == TerminalColors::Red)
        stdout_color = rlutil::RED;
    else if(html_color == TerminalColors::Yellow)
        stdout_color = rlutil::YELLOW;
    else if(html_color == TerminalColors::Gray)
        stdout_color = rlutil::GREY;
    else if(html_color == TerminalColors::White)
        stdout_color = rlutil::WHITE;
    else if(html_color == TerminalColors::Default)
        stdout_color = -1;
}

void Terminal::open_tag() {
    stored += format("<font color=\"%s\">", html_color.c_str());
    opened_tag = true;
}

void Terminal::close_tag() {
    stored += "</font>";
    opened_tag = false;
    insert_html_at_end(stored);
    stored.clear();
}

void Terminal::stdout_colored(const std::string& text) {
    if(stdout_color > 0)
        rlutil::setColor(stdout_color);
    cout << text;
    rlutil::resetColor();
}

void Terminal::insert_html_at_end(const std::string &html) {
    QTextCursor prev_cursor = textCursor();
    moveCursor (QTextCursor::End);
    insertHtml(QString::fromStdString(html));
    setTextCursor(prev_cursor);
}

Terminal::~Terminal() {
    terminal_instance = nullptr;
}

