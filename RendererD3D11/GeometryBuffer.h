#pragma once
//
// Created by islam on 23.02.17.
//

#include <IGeometryBuffer.h>

class ID3D11Buffer;

class GeometryBuffer: public IGeometryBuffer {
    ID3D11Buffer *vertex_buffer, *index_buffer;
    int vertex_count, index_count;
public:
    GeometryBuffer(std::vector<DefaultVertex> &&vertices, std::vector<index_t> &&indices);
    void Render();
    ~GeometryBuffer();
};



