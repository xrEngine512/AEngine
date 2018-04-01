#pragma once
//
// Created by islam on 15.03.17.
//


#include <AbstractShader.h>
#include <vulkan/vulkan.h>


namespace ShaderSystem {
    class Shader final : public ShaderSystem::AbstractShader {
        VkPipeline              pipeline              = nullptr;
        VkPipelineLayout        pipeline_layout       = nullptr;
        VkDescriptorSet         descriptor_set        = nullptr;
        VkDescriptorSetLayout   descriptor_set_layout = nullptr;

        struct UniformBuffer {
            VkDeviceMemory          memory      = nullptr;
            VkBuffer                buffer      = nullptr;
            VkDescriptorBufferInfo  descriptor  = {};
        };

        struct UniformBufferPack {
            UniformBuffer scene_constants, per_frame_buffer, per_object_buffer;
        } VS_uniform_buffer_pack, PS_uniform_buffer_pack;

    public:
        Shader(const std::string & acs_filename, const ShaderDesc & desc);

        ~Shader();

        void render(uint32_t index_count, const ShaderSystem::GenericStruct & aStruct) const override;

        bool update_scene_constants_buffers() override;

        bool update_per_frame_buffers() override;

        bool update_per_object_buffers() override;

    private:
        void from_acs_file(const std::string & filename);

        void handle_input_layout(const ShaderElement & element);

        void initialize_pipeline_layout();

        void destroy_pipeline_layout();

        void initialize_pipeline(const std::vector<VkPipelineShaderStageCreateInfo> & stages);

        void destroy_pipeline();

        void initialize_uniform_buffers();

        void initialize_uniform_buffer(UniformBuffer & buffer, MemoryMultiplexer * memory, VkMemoryPropertyFlags memory_flags);

        void initialize_uniform_buffer_pack(UniformBufferPack & uniform_buffer_pack, BufferPack & buffer_pack);

        void destroy_uniform_buffer_pack(UniformBufferPack & uniform_buffer_pack);

        void destroy_uniform_buffers();

        void initialize_descriptor_set();

        void destroy_descriptor_set();

        void update_uniform_buffer(const UniformBuffer & buffer, const MemoryMultiplexer * multiplexer) const;

        VkPipelineShaderStageCreateInfo load_shader_stage(const ShaderElement & element);

        static VkShaderModule load_module(const void * data, uint64_t size);
    };
}


