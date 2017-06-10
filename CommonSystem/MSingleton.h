//
// Created by islam on 18.03.17.
//

#pragma once

template <class T>
class MSingleton {
public:
    static T& instance() {
        static T instance;
        return instance;
    }
};
