#pragma once
//
// Created by islam on 15.03.17.
//


#include <AbstractShader.h>
#include <vulkan/vulkan.h>


class Shader: public ShaderSystem::AbstractShader {
    std::vector<VkPipelineShaderStageCreateInfo> stages;        // TODO: remove - need only while loading
    VkPipeline pipeline = nullptr;


public:
    Shader(const std::string& acs_filename, const ShaderDesc& desc);
    bool render(uint32_t indexCount, const ShaderSystem::GenericStruct &aStruct) override;

public:
    bool UpdateSceneConstantsBuffers() override;
    bool UpdatePerFrameBuffers() override;

protected:
    void handle_input_layout(const ShaderElement &element) override;
    void handle_compiled_shader(const ShaderElement &element) override;

private:
    static VkShaderModule load_module(const void* data, uint64_t size);
};



