#pragma once
//
// Created by islam on 23.02.17.
//
#include "IShader.h"
#include "ITexture2D.h"
#include "IGeometryBuffer.h"
#include "IExternalMaterial.h"
#include "IInternalMaterial.h"
#include "VertexTypes.h"
#include "AABB.h"

#include "ITextEngine.h"
#include "IGraphicsAPI.h"

#include <string>
#include <vector>


struct ShaderDesc;

/**
 * @brief: abstract factory interface that hides implementation and generalizes API - dependent objects' creation
 *          -- get methods return Singletons
 *          -- create methods return Resources
 */
interface IObjectFactory {
    virtual ITextEngine* getTextEngine() = 0;
    virtual IGraphicsAPI* getGraphicsAPI() = 0;

    virtual ShaderSystem::IShaderPtr createShader(const std::string& acs_filename, const ShaderDesc& description) = 0;
    virtual ITexture2DPtr createTexture(const std::string& file_path) = 0;
    virtual IGeometryBufferPtr createGeometryBuffer(std::vector<DefaultVertex>&& vertices, std::vector<index_t>&& indices, const AABB& aabb = AABB()) = 0;
    virtual IInternalMaterialPtr createMaterial(const std::string& shader, const std::vector<std::string> &textures = {}) = 0;

    virtual ~IObjectFactory(){};
};

IObjectFactory* getObjectFactory();

