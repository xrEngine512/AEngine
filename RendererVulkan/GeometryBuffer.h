#pragma once
//
// Created by islam on 15.03.17.
//


#include <IGeometryBuffer.h>
#include <VertexTypes.h>
#include <AABB.h>

#include <vulkan/vulkan.h>
#include <vector>


class GeometryBuffer: public IGeometryBuffer {
    VkBuffer            vertex_buffer           = nullptr,
                        index_buffer            = nullptr;
    VkDeviceMemory      vertex_buffer_memory    = nullptr,
                        index_buffer_memory     = nullptr;
    uint32_t            index_count             = 0;
    AABB                bounding_box            = {};

    std::pair<VkBuffer, VkDeviceMemory> initialize_buffer(VkBuffer&, VkDeviceMemory&, VkBufferUsageFlags, VkDeviceSize, const void* data );
public:
    GeometryBuffer(std::vector<DefaultVertex> &&vertices, std::vector<index_t> &&indices, const AABB &aabb);
    uint32_t render() override;
    ~GeometryBuffer();
};



