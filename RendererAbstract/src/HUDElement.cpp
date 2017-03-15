#include "HUDElement.h"
#include "VertexTypes.h"
#include "ApproxSystemTools.h"
#include "IObjectFactory.h"
#include "ObjMeshLoader.h"
#include "ShaderPool.h"

using namespace std;
using namespace Math;

inline vector<DefaultVertex> generate_vertices(uint32_t width, uint32_t height) {
    float bottom, top, right, left;
    bottom = static_cast<float>(0) + static_cast<float>(height) / 2;
    top = static_cast<float>(0) - static_cast<float>(height) / 2;
    right = static_cast<float>(0) + static_cast<float>(width) / 2;
    left = static_cast<float>(0) - static_cast<float>(width) / 2;

    return {
        {{left, top, 1.0f, 1.0f},     {0.0f, 0.0f}},
        {{right, bottom, 1.0f, 1.0f}, {1.0f, 1.0f}},
        {{left, bottom, 1.0f, 1.0f},  {0.0f, 1.0f}},
        {{left, top, 1.0f, 1.0f},     {0.0f, 0.0f}},
        {{right, top, 1.0f, 1.0f},    {1.0f, 0.0f}},
        {{right, bottom, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };
}

inline vector<index_t> get_indices() {
    static constexpr initializer_list<index_t> indices = {0, 1, 2, 3, 4, 5};
    return indices;
}

HUDElement::HUDElement(const string& texture, unsigned int width, unsigned int height):
RenderObject(
    getObjectFactory()->createGeometryBuffer(generate_vertices(width, height), get_indices()),
    getObjectFactory()->createMaterial("", {texture})
)
{
}

bool Initialize()
{
    auto m_orthoview =
    {
        -1,0,0,0,
        0,1,0,0,
        0,0,-1,0,
        0,0,5,1
    };
    return true;
}

HUDElement::~HUDElement()
{
}
