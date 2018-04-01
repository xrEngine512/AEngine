//
// Created by islam on 15.03.17.
//

#include "Shader.h"
#include "ObjectFactory.h"
#include "VulkanAPI.h"
#include "ErrorHandling.h"
#include "MemoryMultiplexer.h"

#include <TextureInfo.h>
#include <ShaderParameterInfo.h>
#include <Utility.h>

#include <map>

using namespace std;

namespace ShaderSystem {

    namespace { // utility
        map<ShaderPackElementID, VkShaderStageFlagBits> stage_flags {
            {COMPILED_VS, VK_SHADER_STAGE_VERTEX_BIT},
            {COMPILED_PS, VK_SHADER_STAGE_FRAGMENT_BIT},
            {COMPILED_GS, VK_SHADER_STAGE_GEOMETRY_BIT},
            {COMPILED_CS, VK_SHADER_STAGE_COMPUTE_BIT},
        };
    };  // utility


    void Shader::render(uint32_t index_count, const ShaderSystem::GenericStruct & aStruct) const { using_vulkan
        auto command_buffer = vulkan->get_main_command_buffer();
        vkCmdBindDescriptorSets(
            command_buffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipeline_layout,
            0,
            1,
            &descriptor_set,
            0,
            nullptr
        );

        vkCmdBindPipeline(
            command_buffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipeline
        );

        vkCmdDrawIndexed(command_buffer, index_count, 1, 0, 0, 1);
    }

    Shader::Shader(const std::string &acs_filename, const ShaderDesc &desc) : AbstractShader(desc) {
        from_acs_file(acs_filename);
    }

    Shader::~Shader() {
        destroy_descriptor_set();
        destroy_pipeline();
        destroy_pipeline_layout();
        destroy_uniform_buffers();
    }

    VkPipelineShaderStageCreateInfo Shader::load_shader_stage(const ShaderElement & element) {
        return VkPipelineShaderStageCreateInfo {
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage  = stage_flags[element.m_MetaData.ID],
            .module = load_module(element.Data, element.m_MetaData.size),
            .pName  = "main"
        };
    }

    bool Shader::update_scene_constants_buffers() { using_vulkan
        update_uniform_buffer(VS_uniform_buffer_pack.scene_constants, VS_buffers.scene_constants_buffer.memory);
        update_uniform_buffer(PS_uniform_buffer_pack.scene_constants, PS_buffers.scene_constants_buffer.memory);
        return true;
    }

    bool Shader::update_per_frame_buffers() {
        update_uniform_buffer(VS_uniform_buffer_pack.per_frame_buffer, VS_buffers.per_frame_buffer.memory);
        update_uniform_buffer(PS_uniform_buffer_pack.per_frame_buffer, PS_buffers.per_frame_buffer.memory);
        return true;
    }

    bool Shader::update_per_object_buffers() {
        update_uniform_buffer(VS_uniform_buffer_pack.per_object_buffer, VS_buffers.per_object_buffer.memory);
        update_uniform_buffer(PS_uniform_buffer_pack.per_object_buffer, PS_buffers.per_object_buffer.memory);
        return true;
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

    void Shader::from_acs_file(const std::string & filename) {
        ACSReader reader(filename);
        vector<VkPipelineShaderStageCreateInfo> stages;

        bool end_of_file(false);

        while (!end_of_file)
        {
            const ShaderElement& current_element = reader.NextElement();

            switch (current_element.m_MetaData.ID)
            {
                case SHADER_NAME:
                    m_desc.name.assign((char*)current_element.Data);
                    break;
                case TEXTURE_DESC:
                {
                    ASL::TextureInfo info;
                    current_element.Get(info);
                    m_Settings.TexturesInfo.push_back(info);
                    break;
                }
                case PARAM_DESC:
                {
                    ASL::ShaderParameterInfo info;
                    current_element.Get(info);
                    m_Settings.ParamsInfo.push_back(info);
                    break;
                }
                case COMPILED_VS:
                    initialize_buffers(current_element, VS_buffers);
                case COMPILED_PS:
                    initialize_buffers(current_element, PS_buffers);
                case COMPILED_GS:
                case COMPILED_CS:
                case COMPILED_HS:
                case COMPILED_DS:
                    stages.push_back(load_shader_stage(current_element));
                    break;

                case INPUT_LAYOUT_ELEMENT:
                    handle_input_layout(current_element);
                    break;

                default:
                case ID_NONE:
                    end_of_file = true;
                    break;
            }
        }

        if (stages.empty()) {
            throw approx_exception("No stages found in acs file");
        }

        m_desc.TextureSlots = static_cast<uint8_t>(m_Settings.TexturesInfo.size());

        initialize_uniform_buffers();
        initialize_pipeline_layout();
        initialize_pipeline(stages);
        initialize_descriptor_set();
    }

    void Shader::initialize_pipeline_layout() { using_vulkan
        // Setup layout of descriptors used in this example
        // Basically connects the different shader stages to descriptors for binding uniform buffers, image samplers, etc.
        // So every shader binding should map to one descriptor set layout binding

        // Binding 0: Uniform buffer (Vertex shader)

        vector<VkDescriptorSetLayoutBinding> bindings;

        auto push_binding = [&bindings](const BufferPack::Buffer & buffer, VkShaderStageFlags stage) {
            if (!buffer.memory) {
                return;
            }
            bindings.push_back(VkDescriptorSetLayoutBinding {
                .descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount  = 1,
                .stageFlags       = stage,
                .binding          = buffer.num,
            });
        };

        push_binding(VS_buffers.scene_constants_buffer, VK_SHADER_STAGE_VERTEX_BIT);
        push_binding(VS_buffers.per_frame_buffer, VK_SHADER_STAGE_VERTEX_BIT);
        push_binding(VS_buffers.per_object_buffer, VK_SHADER_STAGE_VERTEX_BIT);

        push_binding(PS_buffers.scene_constants_buffer, VK_SHADER_STAGE_FRAGMENT_BIT);
        push_binding(PS_buffers.per_frame_buffer, VK_SHADER_STAGE_FRAGMENT_BIT);
        push_binding(PS_buffers.per_object_buffer, VK_SHADER_STAGE_FRAGMENT_BIT);

        VkDescriptorSetLayoutCreateInfo descriptorLayout {
            .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext          = nullptr,
            .bindingCount   = static_cast<uint32_t>(bindings.size()),
            .pBindings      = bindings.data(),
        };

        HandleErrors(vkCreateDescriptorSetLayout(vulkan->get_device(), &descriptorLayout, nullptr, &descriptor_set_layout))

        // Create the pipeline layout that is used to generate the rendering pipelines that are based on this descriptor set layout
        // In a more complex scenario you would have different pipeline layouts for different descriptor set layouts that could be reused
        VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo {
            .sType              = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext              = nullptr,
            .setLayoutCount     = 1,
            .pSetLayouts        = &descriptor_set_layout,
        };

        HandleErrors(vkCreatePipelineLayout(vulkan->get_device(), &pPipelineLayoutCreateInfo, nullptr, &pipeline_layout))
    }

    void Shader::destroy_pipeline_layout() { using_vulkan
        if (descriptor_set_layout) {
            vkDestroyDescriptorSetLayout(vulkan->get_device(), descriptor_set_layout, vulkan->get_allocation_callback());
            descriptor_set_layout = nullptr;
        }

        if (pipeline_layout) {
            vkDestroyPipelineLayout(vulkan->get_device(), pipeline_layout, vulkan->get_allocation_callback());
            pipeline_layout = nullptr;
        }
    }

    void Shader::initialize_pipeline(const vector<VkPipelineShaderStageCreateInfo> & stages) { using_vulkan
        VkPipelineInputAssemblyStateCreateInfo inputAssembly_state {
            .sType                      = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology                   = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
            .primitiveRestartEnable     = VK_FALSE
        };

        VkPipelineRasterizationStateCreateInfo rasterization_state {
            .sType                      = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .polygonMode                = VK_POLYGON_MODE_FILL,
            .cullMode                   = VK_CULL_MODE_BACK_BIT,
            .frontFace                  = VK_FRONT_FACE_CLOCKWISE,
            .depthClampEnable           = VK_FALSE,
            .rasterizerDiscardEnable    = VK_FALSE,
            .depthBiasEnable            = VK_FALSE,
            .lineWidth                  = 1.0f,
        };

        VkPipelineColorBlendAttachmentState blend_attachment_state[] {
            {
                .colorWriteMask = 0xf,
                .blendEnable = VK_FALSE,
            }
        };

        VkPipelineColorBlendStateCreateInfo color_blend_state {
            .sType              = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .attachmentCount    = static_size(blend_attachment_state),
            .pAttachments       = blend_attachment_state,
        };

        VkPipelineViewportStateCreateInfo viewport_state {
            .sType          = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount  = 1,
            .scissorCount   = 1,
        };

        array<VkDynamicState, 2> enabled_dynamic_states {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
        };

        VkPipelineDynamicStateCreateInfo dynamic_state {
            .sType              = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .pDynamicStates     = enabled_dynamic_states.data(),
            .dynamicStateCount  = static_cast<uint32_t>(enabled_dynamic_states.size()),
        };

        VkPipelineDepthStencilStateCreateInfo depth_stencil_state {
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .depthTestEnable        = VK_TRUE,
            .depthWriteEnable       = VK_TRUE,
            .depthCompareOp         = VK_COMPARE_OP_LESS_OR_EQUAL,
            .depthBoundsTestEnable  = VK_FALSE,
            .back.failOp            = VK_STENCIL_OP_KEEP,
            .back.passOp            = VK_STENCIL_OP_KEEP,
            .back.compareOp         = VK_COMPARE_OP_ALWAYS,
            .stencilTestEnable      = VK_FALSE,
        };
        depth_stencil_state.front = depth_stencil_state.back;

        // Multi sampling state for AA (unused)
        VkPipelineMultisampleStateCreateInfo multisample_state {
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples   = VK_SAMPLE_COUNT_1_BIT,
            .pSampleMask            = nullptr,
        };

        VkVertexInputBindingDescription vertex_input_binding {
            .binding    = 0,
            .stride     = sizeof(DefaultVertex::Data),
            .inputRate  = VK_VERTEX_INPUT_RATE_VERTEX,
        };

        // TODO: generate dynamically
        VkVertexInputAttributeDescription vertex_input_attributes[] {
            {   // position
                .binding    = 0,
                .location   = 0,
                .format     = VK_FORMAT_R32G32B32A32_SFLOAT,
                .offset     = offsetof(DefaultVertex::Data, position),
            },
            {   // texture
                .binding    = 0,
                .location   = 1,
                .format     = VK_FORMAT_R32G32_SFLOAT,
                .offset     = offsetof(DefaultVertex::Data, texture),
            },
            {   // normal
                .binding    = 0,
                .location   = 2,
                .format     = VK_FORMAT_R32G32B32_SFLOAT,
                .offset     = offsetof(DefaultVertex::Data, normal),
            }
        };

        VkPipelineVertexInputStateCreateInfo vertex_input_state {
            .sType                              = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .vertexBindingDescriptionCount      = 1,
            .pVertexBindingDescriptions         = &vertex_input_binding,
            .vertexAttributeDescriptionCount    = static_size(vertex_input_attributes),
            .pVertexAttributeDescriptions       = vertex_input_attributes,
        };

        VkPipelineInputAssemblyStateCreateInfo input_assembly_state {
            .sType      = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology   = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        };

        VkGraphicsPipelineCreateInfo info {
            .sType                  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .layout                 = pipeline_layout,
            .renderPass             = vulkan->get_render_pass(),
            .stageCount             = static_cast<uint32_t>(stages.size()),
            .pStages                = stages.data(),
            .pVertexInputState      = &vertex_input_state,
            .pInputAssemblyState    = &input_assembly_state,
            .pRasterizationState    = &rasterization_state,
            .pColorBlendState       = &color_blend_state,
            .pMultisampleState      = &multisample_state,
            .pViewportState         = &viewport_state,
            .pDepthStencilState     = &depth_stencil_state,
            .pDynamicState          = &dynamic_state,
        };

        HandleErrors(vkCreateGraphicsPipelines(
            vulkan->get_device(),
            vulkan->get_pipeline_cache(),
            1,
            &info,
            vulkan->get_allocation_callback(),
            &pipeline
        ));
    }

    void Shader::destroy_pipeline() { using_vulkan
        if (pipeline) {
            vkDestroyPipeline(vulkan->get_device(), pipeline, vulkan->get_allocation_callback());
            pipeline = nullptr;
        }
    }

    void Shader::initialize_uniform_buffer(UniformBuffer & buffer, MemoryMultiplexer * memory, VkMemoryPropertyFlags memory_flags) { using_vulkan
        if (!memory) {
            return; // this buffer is not needed skipping
        }

        VkBufferCreateInfo info {
            .sType  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size   = memory->get_size(),
            .usage  = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
        };

        HandleErrors(vkCreateBuffer(vulkan->get_device(), &info, vulkan->get_allocation_callback(), &buffer.buffer))

        VkMemoryRequirements memory_requirements {};
        vkGetBufferMemoryRequirements(vulkan->get_device(), buffer.buffer, &memory_requirements);

        VkMemoryAllocateInfo allocate_info {
            .sType              = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize     = memory_requirements.size,
            .memoryTypeIndex    = vulkan->find_memory_type_index(memory_requirements, memory_flags)
        };

        HandleErrors(vkAllocateMemory(vulkan->get_device(), &allocate_info, vulkan->get_allocation_callback(), &buffer.memory))
        HandleErrors(vkBindBufferMemory(vulkan->get_device(), buffer.buffer, buffer.memory, 0));

        buffer.descriptor.buffer = buffer.buffer;
        buffer.descriptor.offset = 0;
        buffer.descriptor.range  = memory->get_size();
    }

    void Shader::initialize_uniform_buffer_pack(Shader::UniformBufferPack & uniform_buffer_pack, BufferPack & buffer_pack) {
        // Get the memory type index that supports host visible memory access
        // Most implementations offer multiple memory types and selecting the correct one to allocate memory from is crucial
        // We also want the buffer to be host coherent so we don't have to flush (or sync after every update.
        // Note: This may affect performance so you might not want to do this in a real world application that updates buffers on a regular base

        initialize_uniform_buffer(
            uniform_buffer_pack.scene_constants,
            buffer_pack.scene_constants_buffer.memory,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        initialize_uniform_buffer(
            uniform_buffer_pack.per_frame_buffer,
            buffer_pack.per_frame_buffer.memory,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        initialize_uniform_buffer(
            uniform_buffer_pack.per_object_buffer,
            buffer_pack.per_object_buffer.memory,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );
    }

    void Shader::initialize_uniform_buffers() {
        initialize_uniform_buffer_pack(VS_uniform_buffer_pack, VS_buffers);
        initialize_uniform_buffer_pack(PS_uniform_buffer_pack, PS_buffers);
    }

    void Shader::destroy_uniform_buffer_pack(UniformBufferPack & uniform_buffer_pack) { using_vulkan
        vkDestroyBuffer(vulkan->get_device(), uniform_buffer_pack.scene_constants.buffer, vulkan->get_allocation_callback());
        vkFreeMemory(vulkan->get_device(), uniform_buffer_pack.scene_constants.memory, vulkan->get_allocation_callback());

        vkDestroyBuffer(vulkan->get_device(), uniform_buffer_pack.per_frame_buffer.buffer, vulkan->get_allocation_callback());
        vkFreeMemory(vulkan->get_device(), uniform_buffer_pack.per_frame_buffer.memory, vulkan->get_allocation_callback());

        vkDestroyBuffer(vulkan->get_device(), uniform_buffer_pack.per_object_buffer.buffer, vulkan->get_allocation_callback());
        vkFreeMemory(vulkan->get_device(), uniform_buffer_pack.per_object_buffer.memory, vulkan->get_allocation_callback());
    }

    void Shader::destroy_uniform_buffers() {
        destroy_uniform_buffer_pack(VS_uniform_buffer_pack);
        destroy_uniform_buffer_pack(PS_uniform_buffer_pack);
    }

    void Shader::initialize_descriptor_set() { using_vulkan
        // Allocate a new descriptor set from the global descriptor pool
        VkDescriptorSetAllocateInfo alloc_info {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool = vulkan->get_descriptor_pool(),
            .descriptorSetCount = 1,
            .pSetLayouts = &descriptor_set_layout,
        };

        HandleErrors(vkAllocateDescriptorSets(vulkan->get_device(), &alloc_info, &descriptor_set));

        // Update the descriptor set determining the shader binding points
        // For every binding point used in a shader there needs to be one
        // descriptor set matching that binding point

        vector<VkWriteDescriptorSet> write_descriptor_sets;

        auto push_write = [&write_descriptor_sets, this](const UniformBuffer & uniform_buffer, const BufferPack::Buffer & buffer){
            if (!uniform_buffer.buffer) {
                return;
            }
            write_descriptor_sets.push_back(VkWriteDescriptorSet {
                .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .dstSet             = descriptor_set,
                .descriptorCount    = 1,
                .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .pBufferInfo        = &uniform_buffer.descriptor,
                .dstBinding         = buffer.num,
            });
        };

        push_write(VS_uniform_buffer_pack.scene_constants,   VS_buffers.scene_constants_buffer);
        push_write(VS_uniform_buffer_pack.per_frame_buffer,  VS_buffers.per_frame_buffer);
        push_write(VS_uniform_buffer_pack.per_object_buffer, VS_buffers.per_object_buffer);

        push_write(PS_uniform_buffer_pack.scene_constants,   PS_buffers.scene_constants_buffer);
        push_write(PS_uniform_buffer_pack.per_frame_buffer,  PS_buffers.per_frame_buffer);
        push_write(PS_uniform_buffer_pack.per_object_buffer, PS_buffers.per_object_buffer);


        vkUpdateDescriptorSets(
            vulkan->get_device(),
            static_cast<uint32_t>(write_descriptor_sets.size()),
            write_descriptor_sets.data(),
            0, nullptr
        );
    }

    void Shader::destroy_descriptor_set() { using_vulkan
        if (descriptor_set) {
            vkFreeDescriptorSets(vulkan->get_device(), vulkan->get_descriptor_pool(), 1, &descriptor_set);
            descriptor_set = nullptr;
        }
    }

    void Shader::update_uniform_buffer(const Shader::UniformBuffer & buffer, const MemoryMultiplexer * multiplexer) const { using_vulkan
        void * data;
        HandleErrors(vkMapMemory(vulkan->get_device(), buffer.memory, 0, multiplexer->get_size(), 0, &data))
        multiplexer->write_to(data);
        vkUnmapMemory(vulkan->get_device(), buffer.memory);
    }

}
