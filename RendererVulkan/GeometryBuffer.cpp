//
// Created by islam on 15.03.17.
//

#include "GeometryBuffer.h"

#include "ObjectFactory.h"
#include "VulkanAPI.h"
#include "ErrorHandling.h"

template <class Container>
size_t true_size(const Container container) {
    return container.size() * sizeof(typename Container::value_type);
}


GeometryBuffer::GeometryBuffer(std::vector<DefaultVertex> &&vertices, std::vector<index_t> &&indices, const AABB &aabb):
    bounding_box(aabb), index_count(static_cast<uint32_t>(indices.size()))
{
    auto queue_family_index = vulkan->get_queue_family_index();
    {   /** Vertex buffer creation **/
        VkBufferCreateInfo buffer_info{
            .sType                      = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .usage                      = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            .queueFamilyIndexCount      = 1,
            .pQueueFamilyIndices        = &queue_family_index,
            .size                       = true_size(vertices)
        };
        HandleErrors(vkCreateBuffer(vulkan->get_device(), &buffer_info, vulkan->get_allocation_callback(), &vertex_buffer))
    }
    {   /** Index buffer creation **/
        VkBufferCreateInfo buffer_info{
            .sType                      = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .usage                      = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            .queueFamilyIndexCount      = 1,
            .pQueueFamilyIndices        = &queue_family_index,
            .size                       = true_size(indices)
        };
        HandleErrors(vkCreateBuffer(vulkan->get_device(), &buffer_info, vulkan->get_allocation_callback(), &index_buffer))
    }
}

uint32_t GeometryBuffer::render() {
    VkDeviceSize offsets[1] { 0 };
    vkCmdBindVertexBuffers(vulkan->get_main_command_buffer(), 0, 1, &vertex_buffer, offsets);
    vkCmdBindIndexBuffer(vulkan->get_main_command_buffer(), index_buffer, 0, VK_INDEX_TYPE_UINT32);
    return index_count;
}

GeometryBuffer::~GeometryBuffer() {
    vkDestroyBuffer(vulkan->get_device(), index_buffer, vulkan->get_allocation_callback());
    vkDestroyBuffer(vulkan->get_device(), vertex_buffer, vulkan->get_allocation_callback());
}
