#pragma once
#include "IMeshLoader.h"
#include "VertexTypes.h"

#include <ApproxMath.h>
#include <memory>
#include <vector>


class ObjMeshLoader : public IMeshLoader
{
public:
    IGeometryBufferPtr Load(const std::experimental::string_view &mesh_file_name) override;
};

