//
// Created by islam on 23.02.17.
//

#include "GeometryBuffer.h"
#include "D3D11Common.h"

#include <ApproxSystemErrors.h>

GeometryBuffer::GeometryBuffer(std::vector<DefaultVertex> &&vertices, std::vector<index_t> &&indices) {
    vertex_buffer = D3D11Common::InitializeStaticVertexBuffer(device, vertices.data(), vertices.size());
    if (!vertex_buffer)
        throw ApproxException("Не удалось инициализировать буфер вершин");
    index_buffer = D3D11Common::InitializeIndexBuffer(device, indices.data(), indices.size());
    if (!index_buffer) {
        vertex_buffer->Release();
        throw ApproxException("Не удалось инициализировать индексный буфер");
    }
}

void GeometryBuffer::Render() {
    unsigned int stride = sizeof(DefaultVertex);
    unsigned int offset = 0;

    // Set the vertex buffer to active in the input assembler so it can be rendered.
    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

~GeometryBuffer::GeometryBuffer() {
    vertex_buffer->Release();
    index_buffer->Release();
}
