//
// Created by islam on 11.03.17.
//

#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <functional>

size_t count_size(const std::string& arg) {
    return arg.size();
}

size_t count_size(std::string& arg) {
    return arg.size();
}

size_t count_size(const char* arg) {
    return strlen(arg);
}

size_t count_size(char* arg) {
    return strlen(arg);
}

template <class T>
size_t count_size(T arg) {
    return count_size(std::to_string(arg));
}

template <class Head, class ... Tail>
size_t count_size(Head arg, Tail... args) {
    return count_size(arg) + count_size(args...);
}

template <class ... Args>
std::string format(const std::string& template_string, Args... args) {
    std::vector<char> res;
    res.reserve(template_string.size() + count_size(args...) + 1);
    sprintf(res.data(), template_string.c_str(), args...);
    return res.data();
}

static inline void replace_substrings(std::string& str, const std::string& what, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(what, start_pos)) != std::string::npos) {
        str.replace(start_pos, what.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
}

static inline std::string replace_substrings(const std::string& str, const std::string& what, const std::string& to) {
    std::string res(str);
    replace_substrings(res, what, to);
    return res;
}
