//
// Created by islam on 08.03.17.
//

#include "AbstractObjectFactory.h"
#include "Material.h"

using namespace std;

IInternalMaterialPtr AbstractObjectFactory::createMaterial(const string& shader, const std::vector<std::string> &textures) {
    return make_shared<Material>(shader, textures);
}
