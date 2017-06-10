//
// Created by islam on 23.02.17.
//

#pragma once

#include <mutex>

template <class T>
class thread_safe: public T {
    mutable std::mutex mtx;
public:
    template <class ...Ts>
    thread_safe(Ts ...args): T(args...) {}

    std::unique_lock<std::mutex> lock() const {
        auto lock = std::unique_lock<std::mutex>(mtx);
        return std::move(lock);
    }

    std::unique_lock<std::mutex> try_lock() const {
        auto lock = std::unique_lock<std::mutex>(mtx, std::try_to_lock_t());
        return std::move(lock);
    }

    T operator=(const T& other) {
        auto lock = this->lock();
        return T::operator=(other);
    }
};
