#pragma once
//
// Created by islam on 08.03.17.
//

#include <IObjectFactory.h>


class AbstractObjectFactory: public IObjectFactory {
public:
    IInternalMaterialPtr createMaterial(const std::string &shader, const std::vector<std::string> &textures) override;
};



