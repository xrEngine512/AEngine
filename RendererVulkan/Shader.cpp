//
// Created by islam on 15.03.17.
//

#include "Shader.h"
#include "ObjectFactory.h"
#include "VulkanAPI.h"
#include "ErrorHandling.h"

#include <map>

using namespace std;

map<ShaderPackElementID, VkShaderStageFlagBits> stage_flags {
    {COMPILED_VS, VK_SHADER_STAGE_VERTEX_BIT},
    {COMPILED_PS, VK_SHADER_STAGE_FRAGMENT_BIT},
    {COMPILED_GS, VK_SHADER_STAGE_GEOMETRY_BIT},
    {COMPILED_CS, VK_SHADER_STAGE_COMPUTE_BIT},
};


bool Shader::render(uint32_t indexCount, const ShaderSystem::GenericStruct &aStruct) {
    return false;
}

Shader::Shader(const std::string &acs_filename, const ShaderDesc &desc) : AbstractShader(acs_filename, desc) {
    VkGraphicsPipelineCreateInfo info {
        .sType      = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,

    };
}

void Shader::handle_compiled_shader(const ShaderElement &element) {
    VkPipelineShaderStageCreateInfo info {
        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage  = stage_flags[element.m_MetaData.ID],
        .module = load_module(element.Data, element.m_MetaData.size),
        .pName  = "main"
    };

}

bool Shader::UpdateSceneConstantsBuffers() {
    return false;
}

bool Shader::UpdatePerFrameBuffers() {
    return false;
}

void Shader::handle_input_layout(const ShaderElement &element) {

}

VkShaderModule Shader::load_module(const void* data, uint64_t size) { using_vulkan
    VkShaderModuleCreateInfo module_info {
        .sType          = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize       = size,
        .pCode          = (const uint32_t*)data
    };

    VkShaderModule module = nullptr;
    HandleErrors(vkCreateShaderModule(vulkan->get_device(), &module_info, vulkan->get_allocation_callback(), &module))
    return module;
}


