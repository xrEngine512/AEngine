#pragma once
//
// Created by islam on 01.10.17.
//


#include "PipelineSource.h"

namespace ASL {
    struct PipelineProject {
        std::string     subsystem;
        std::string     language;
        std::string     name;

        PipelineSource  source;

        MSGPACK_DEFINE(subsystem, language, name, source);
    };
}


