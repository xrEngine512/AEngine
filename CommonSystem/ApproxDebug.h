//
// Created by islam on 17.02.17.
//

#pragma once

#include <iostream>

#ifdef _DEBUG
#define APPROX_DEBUG(message) std::cout << message << std::endl;
#else
#define APPROX_DEBUG(message)
#endif

