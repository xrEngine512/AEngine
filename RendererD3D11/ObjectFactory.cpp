//
// Created by islam on 25.02.17.
//

#include "ObjectFactory.h"
#include "GeometryBuffer.h"

using namespace std;

IGeometryBufferPtr ObjectFactory::createGeometryBuffer(std::vector<DefaultVertex>&& vertices, std::vector<index_t>&& indices) {
    return make_shared<GeometryBuffer>(move(vertices), move(indices));
}
