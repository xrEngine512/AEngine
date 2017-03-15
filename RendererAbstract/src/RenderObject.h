#pragma once


#include "IGeometryBuffer.h"
#include "IInternalMaterial.h"
#include "IInternalRenderObject.h"
#include "IExternalRenderObject.h"

#include <vector>

class AbstractScene;

class RenderObject : public IInternalRenderObject, public IExternalRenderObject
{
    friend class AbstractScene;
    std::vector<IGeometryBufferPtr> buffers;
    std::vector<IInternalMaterialPtr> materials;
    AABB bounding_box;
    uint32_t m_ID;
    bool visible;
	Math::AVector3 position, orientation, scale;
    RenderObject(const std::string &modelFileName, const std::string &shader = "", const Math::AVector3 &position = {},
                 const Math::AVector3 &orientation = {}, const Math::AVector3 &scale = {});
	void set_is_visible() override;

protected:
    RenderObject(const Math::AVector3 &position = {},
                 const Math::AVector3 &orientation = {}, const Math::AVector3 &scale = {});
	RenderObject(const IGeometryBufferPtr &geometry, const IInternalMaterialPtr &material, const Math::AVector3 &position = {},
                 const Math::AVector3 &orientation = {}, const Math::AVector3 &scale = {});

public:
    bool is_visible() const;
	bool render(MatInterface::MaterialInterface &)override;
    std::vector<std::pair<IInternalMaterialRef, IGeometryBufferRef>> get_render_info() const override;
    void SetCoord(float x, float y, float z)override;
    void SetRotation(float x, float y, float z)override;
	Math::AVector GetPoint()override;
	Math::AVector3 GetCoord()override;
    const AABB& aabb() const override;
    unsigned int id()override;
    void set_id(unsigned int)override;
    IExternalMaterialRef material(uint8_t id) override;
    virtual ~RenderObject(void);
};

