//
// Created by islam on 23.02.17.
//

#pragma once

template <class T>
class Service {
public:
    static T& Instance() {
        static T instance;
        return instance;
    }
};
