//
// Created by islam on 23.02.17.
//

#pragma once

#include <mutex>

template <class T>
class thread_safe: public T {
    mutable std::mutex mutex;
public:
    std::unique_lock<std::mutex> lock() const {
        auto lock = std::unique_lock<std::mutex>{mutex};
        lock.lock();
        return std::move(lock);
    }

    std::unique_lock<std::mutex> try_lock() const {
        auto lock = std::unique_lock<std::mutex>{mutex};
        lock.try_lock();
        return std::move(lock);
    }

    T operator=(const T& other) {
        auto lock = this->lock();
        return T::operator=(other);
    }
};
