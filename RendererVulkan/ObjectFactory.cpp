//
// Created by islam on 11.03.17.
//

#include "ObjectFactory.h"
#include "VulkanAPI.h"
#include "GeometryBuffer.h"

using namespace std;

ITextEngine* ObjectFactory::getTextEngine() {
    return nullptr;
}

IGraphicsAPI* ObjectFactory::getGraphicsAPI() {
    return getLocalGraphicsAPI();
}

ShaderSystem::IShaderPtr ObjectFactory::createShader(const std::string &acs_filename, const ShaderDesc &description) {
    return nullptr;
}

ITexture2DPtr ObjectFactory::createTexture(const std::string &file_path) {
    return nullptr;
}

IGeometryBufferPtr
ObjectFactory::createGeometryBuffer(std::vector<DefaultVertex> &&vertices, std::vector<index_t> &&indices,
                                    const AABB &aabb) {
    return make_shared<GeometryBuffer>(move(vertices), move(indices), aabb);
}

ObjectFactory::~ObjectFactory() {
    if (graphics_api)
        delete graphics_api;

    if (text_engine)
        delete text_engine;
}

VulkanAPI *ObjectFactory::getLocalGraphicsAPI() {
    if (!graphics_api) {
        graphics_api = new VulkanAPI();
    }
    return graphics_api;
}

IObjectFactory* getObjectFactory() {
    return getLocalObjectFactory();
}

ObjectFactory* getLocalObjectFactory() {
    static ObjectFactory instance;
    return &instance;
}
