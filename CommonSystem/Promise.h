//
// Created by islam on 26.02.17.
//

#pragma once

#include <Logger.h>

#include <mutex>
#include <chrono>
#include <functional>
#include <condition_variable>

using namespace std::chrono_literals;


template <typename ...Args> class Promise;


template <typename ...Args>
class Fulfill {
    friend class Promise<Args...>;
    typedef std::function<void(Args...)> Functor_t;

    Functor_t callback = nullptr;
    bool wait_for_promise = true;
    std::condition_variable condition_variable;
    std::mutex mutex;

    void notify_promise_destroyed() {
        {
            std::unique_lock<std::mutex> lock(mutex);
            wait_for_promise = false;
        }
        condition_variable.notify_one();
    }

    // expected to be called from different of operator() thread
    void set_callback(Functor_t fn) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            callback = fn;
            wait_for_promise = false;
        }
        condition_variable.notify_one();
    }
public:
    // waits whether promise is destroyed without setting a callback (no 'then' was called) or 'then' is called
    void operator()(Args... args) {
        std::unique_lock<std::mutex> lock(mutex);
        if (condition_variable.wait_for(lock, 100ms, [&]{return !wait_for_promise;})) {
            if (callback)
                callback(args...);
        } else {
            Logger::instance().warning("promise timeout, wasted 100ms");
        }
    }
};

/**
 *
 * @brief: A JavaScript-like Promise
 */
template <typename ...Args>
class Promise {
    Promise() {
        fulfill = std::make_shared<Fulfill_t>();
    }
public:
    typedef Fulfill<Args...>                Fulfill_t;
    typedef typename Fulfill_t::Functor_t   Functor_t;
    typedef std::shared_ptr<Fulfill_t>      Fulfill_ptr_t;

    Promise(const Promise&) = delete;

    Promise(Promise&& other) {
        this->fulfill = other.fulfill;
        other.fulfill = nullptr;
    }

    ~Promise() {
        if (fulfill)
            fulfill->notify_promise_destroyed();
    }

    /**
     * @attention 'then' can be called only once per Promise instance, all next calls will be ignored
     * @param fn: callback
     */
    void then(Functor_t fn) {
        if (fulfill) {
            fulfill->set_callback(fn);
            fulfill = nullptr;
        }
    }

private:
    template <typename ..._Args>
    friend std::pair<Promise<_Args...>, std::shared_ptr<Fulfill<_Args...>>> make_promise();
    friend class Fulfill<Args...>;

    Fulfill_ptr_t fulfill;
};

template <typename ...Args>
std::pair<Promise<Args...>, std::shared_ptr<Fulfill<Args...>>> make_promise() {
    auto promise = Promise<Args...>();
    auto fulfill = promise.fulfill;
    return std::make_pair(std::move(promise), std::shared_ptr<Fulfill<Args...>>(fulfill));
};
