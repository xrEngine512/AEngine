//
// Created by islam on 15.03.17.
//

#include "GeometryBuffer.h"

#include "ObjectFactory.h"
#include "VulkanAPI.h"
#include "ErrorHandling.h"

#include <cstring>

using namespace std;


template <class Container>
size_t byte_size(const Container container) {
    return container.size() * sizeof(typename Container::value_type);
}

/**
 * @param vertices: array of vertices
 * @param indices:  array of indices
 * @param aabb:     Axis-aligned bounding box
 */
GeometryBuffer::GeometryBuffer(std::vector<DefaultVertex> &&vertices, std::vector<index_t> &&indices, const AABB &aabb):
    bounding_box(aabb), index_count(static_cast<uint32_t>(indices.size()))
{ using_vulkan

    VkDeviceSize vertex_buffer_size = byte_size(vertices), index_buffer_size = byte_size(indices);

    VkBuffer tmp_vertex_buffer, tmp_index_buffer;
    VkDeviceMemory tmp_vertex_memory, tmp_index_memory;

    tie(tmp_vertex_buffer, tmp_vertex_memory) = initialize_buffer(
        vertex_buffer, vertex_buffer_memory, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertex_buffer_size, vertices.data()
    );

    tie(tmp_index_buffer, tmp_index_memory) = initialize_buffer(
        index_buffer, index_buffer_memory, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, index_buffer_size, indices.data()
    );

    auto command_buffer = vulkan->begin_transfer();

    /** Copy vertex buffer **/
    {
        VkBufferCopy copy_region{
            .size = vertex_buffer_size,
        };
        vkCmdCopyBuffer(command_buffer, tmp_vertex_buffer, vertex_buffer, 1, &copy_region);
    }

    /** Copy index buffer **/
    {
        VkBufferCopy copy_region{
            .size = index_buffer_size,
        };
        vkCmdCopyBuffer(command_buffer, tmp_index_buffer, index_buffer, 1, &copy_region);
    }

    vulkan->end_transfer();

    vkDestroyBuffer(vulkan->get_device(), tmp_vertex_buffer, vulkan->get_allocation_callback());
    vkFreeMemory(vulkan->get_device(), tmp_vertex_memory, vulkan->get_allocation_callback());

    vkDestroyBuffer(vulkan->get_device(), tmp_index_buffer, vulkan->get_allocation_callback());
    vkFreeMemory(vulkan->get_device(), tmp_index_memory, vulkan->get_allocation_callback());
}

uint32_t GeometryBuffer::render() { using_vulkan
    VkDeviceSize offsets[] { 0 };
    vkCmdBindVertexBuffers(vulkan->get_main_command_buffer(), 0, 1, &vertex_buffer, offsets);
    vkCmdBindIndexBuffer(vulkan->get_main_command_buffer(), index_buffer, 0, VK_INDEX_TYPE_UINT32);
    return index_count;
}

GeometryBuffer::~GeometryBuffer() { using_vulkan
    vkDestroyBuffer(vulkan->get_device(), index_buffer, vulkan->get_allocation_callback());
    vkDestroyBuffer(vulkan->get_device(), vertex_buffer, vulkan->get_allocation_callback());
}

/**
 * @brief Allocates temporary staging buffers, copies data to them, then sends to another buffer allocated on GPU
 */
pair<VkBuffer, VkDeviceMemory> GeometryBuffer::initialize_buffer(VkBuffer &buffer, VkDeviceMemory &memory, VkBufferUsageFlags usage, VkDeviceSize size, const void *data) {
    using_vulkan

    VkDeviceMemory      tmp_memory;
    VkBuffer            tmp_buffer;

    VkBufferCreateInfo buffer_info {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size  = size,
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT
    };

    HandleErrors(vkCreateBuffer(vulkan->get_device(), &buffer_info, vulkan->get_allocation_callback(), &tmp_buffer))

    VkMemoryRequirements memory_requirements = {};

    vkGetBufferMemoryRequirements(vulkan->get_device(), tmp_buffer, &memory_requirements);

    VkMemoryAllocateInfo memory_allocate_info {
        .sType              = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize     = memory_requirements.size,
        .memoryTypeIndex    = vulkan->find_memory_type_index(memory_requirements, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
    };
    HandleErrors(vkAllocateMemory(vulkan->get_device(), &memory_allocate_info, vulkan->get_allocation_callback(), &tmp_memory))
    /** Map memory and copy to it **/
    void* mapped_memory_ptr = nullptr;
    HandleErrors(vkMapMemory(vulkan->get_device(), tmp_memory, 0, size, 0, &mapped_memory_ptr))
    memcpy(mapped_memory_ptr, data, size);
    vkUnmapMemory(vulkan->get_device(), tmp_memory);

    HandleErrors(vkBindBufferMemory(vulkan->get_device(), tmp_buffer, tmp_memory, 0))


    /** Create GPU located buffer **/
    auto queue_family_index = vulkan->get_queue_family_index();
    {
        buffer_info.usage = usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        buffer_info.queueFamilyIndexCount = 1;
        buffer_info.pQueueFamilyIndices = &queue_family_index;

        HandleErrors(vkCreateBuffer(vulkan->get_device(), &buffer_info, vulkan->get_allocation_callback(), &buffer))
        vkGetBufferMemoryRequirements(vulkan->get_device(), buffer, &memory_requirements);
        memory_allocate_info.allocationSize = memory_requirements.size;
        memory_allocate_info.memoryTypeIndex = vulkan->find_memory_type_index(memory_requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        HandleErrors(vkAllocateMemory(vulkan->get_device(), &memory_allocate_info, vulkan->get_allocation_callback(), &memory))
        HandleErrors(vkBindBufferMemory(vulkan->get_device(), buffer, memory, 0))
    }

    return {tmp_buffer, tmp_memory};
}
