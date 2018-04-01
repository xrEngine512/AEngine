//
// Created by islam on 05.08.17.
//

#pragma once

#include <stdint.h>
#include <functional>

namespace __detail__ {
    class scope_guard {
        std::function<void()> on_scope_exit;

    public:
        scope_guard(std::function<void()> && callback)
            : on_scope_exit(std::move(callback))
        {}

        ~scope_guard() {
            if (on_scope_exit) {
                if (std::uncaught_exception()) {
                    // throwing here will terminate app, i don't want that.
                    try { on_scope_exit(); } catch (...) {}
                } else {
                    on_scope_exit();
                }
            }
        }
    };
}

template <typename T, uint32_t N>
constexpr uint32_t static_size(const T(&)[N]) {
    return N;
};


#define scope_exit(expression) __detail__::scope_guard scope_guard ## __COUNTER__([&]{expression});

