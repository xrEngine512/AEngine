//
// Created by islam on 17.02.17.
//

#pragma once

#include <string>

namespace MatInterface {
    interface IMaterialVariable;

    struct MaterialInterfaceInfo {
        MaterialInterfaceInfo(const std::vector<IMaterialVariable *> &vars, const std::string &ver) : Vars(vars), Ver(ver) {}

        const std::vector<IMaterialVariable *> &Vars;
        const std::string &Ver;
    };
}
