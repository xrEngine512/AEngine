//
// Created by islam on 23.02.17.
//
#pragma once

#include <memory>

namespace {
    interface IGeometryBuffer {
        virtual uint32_t render() = 0;

        virtual ~IGeometryBuffer() {};
    };
}

typedef std::shared_ptr<IGeometryBuffer> IGeometryBufferPtr;
typedef std::weak_ptr<IGeometryBuffer> IGeometryBufferRef;
