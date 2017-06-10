//
// Created by islam on 19.03.17.
//

#pragma once

#include <vector>

#include "IShaderCodeProcessor.h"

namespace ASL {

    std::vector<IShaderCodeProcessorPtr> enumerate_available_processors();

}
