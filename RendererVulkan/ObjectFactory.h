#pragma once
//
// Created by islam on 11.03.17.
//


#include <AbstractObjectFactory.h>


interface ITextEngine;
interface IGraphicsAPI;

class VulkanAPI;


class ObjectFactory: public AbstractObjectFactory {
    ITextEngine* text_engine = nullptr;
    VulkanAPI* graphics_api = nullptr;

public:
    ITextEngine* getTextEngine() override;

    IGraphicsAPI* getGraphicsAPI() override;

    ShaderSystem::IShaderPtr createShader(const std::string &acs_filename, const ShaderDesc &description) override;

    ITexture2DPtr createTexture(const std::string &file_path) override;

    IGeometryBufferPtr createGeometryBuffer(std::vector<DefaultVertex> &&vertices, std::vector<index_t> &&indices,
                                            const AABB &aabb) override;

    VulkanAPI* getLocalGraphicsAPI();

    ~ObjectFactory();
};

ObjectFactory* getLocalObjectFactory();

#define vulkan getLocalObjectFactory()->getLocalGraphicsAPI()
