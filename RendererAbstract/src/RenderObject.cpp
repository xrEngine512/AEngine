#include <ApproxSystemTools.h>
#include <ApproxSystemErrors.h>

#include "IMeshLoader.h"
#include "IObjectFactory.h"
#include "RenderObject.h"
#include "MaterialInterface.h"

using namespace MatInterface;
using namespace Math;
using namespace std;

RenderObject::RenderObject(const string &modelFileName, const string &shader, const AVector3 &position,
                           const AVector3 &orientation, const Math::AVector3 &scale):
    RenderObject(
        getMeshLoader().Load(modelFileName),
        getObjectFactory()->createMaterial(shader),
        position, orientation, scale
    )
{

}

RenderObject::RenderObject(const IGeometryBufferPtr &geometry, const IInternalMaterialPtr &material, const Math::AVector3 &position,
                           const Math::AVector3 &orientation, const Math::AVector3 &scale):
    RenderObject(position, orientation, scale)
{
    buffers.push_back(geometry);
    materials.push_back(material);
}

RenderObject::RenderObject(const AVector3 &position, const AVector3 &orientation, const AVector3 &scale):
    position(position), orientation(orientation), scale(scale), visible(false)
{

}

AVector RenderObject::GetPoint()
{
    return AVector(position, 0);
}

AVector3 RenderObject::GetCoord()
{
    return position;
}

const AABB& RenderObject::aabb() const
{
    return bounding_box;
}

unsigned RenderObject::id()
{
    return m_ID;
}

void RenderObject::set_id(unsigned ID)
{
    m_ID = ID;
}

IExternalMaterialRef RenderObject::material(uint8_t id)
{
    return dynamic_pointer_cast<IExternalMaterial>(materials[id]);
}

void RenderObject::SetCoord(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;
}

void RenderObject::SetRotation(float x, float y, float z)
{
    orientation.x = x;
    orientation.y = y;
    orientation.z = z;
}

bool RenderObject::render(MaterialInterface &mi)
{
    visible = false;
    mi.perObjectData.modelMatrix = AToMatrix(AQuaternion(orientation)) * ATranslationMatrix(AVector3(position.x, position.y, position.z));
	mi.CalculateMVPBeforeTranspose();

    for (auto i = 0; i < min(buffers.size(), materials.size()); ++i)
	    materials[i]->render(buffers[i]->render());
    return true;
}

vector<pair<IInternalMaterialRef, IGeometryBufferRef>> RenderObject::get_render_info() const {
    vector<pair<IInternalMaterialRef, IGeometryBufferRef>> res;
    auto size = std::min(buffers.size(), materials.size());
    res.reserve(size);
    for(int i = 0; i < size; ++i)
        res.push_back({materials[i], buffers[i]});
    return res;
}

RenderObject::~RenderObject(void)
{
}

bool RenderObject::is_visible() const {
    return visible;
}

void RenderObject::set_is_visible() {
    visible = true;
}

