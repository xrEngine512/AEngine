#pragma once
#include "IInternalRenderObject.h"
#include "AABB.h"
#include "Frustum.h"
#include <vector>


namespace MatInterface
{
	class MaterialInterface;
}

class Octree
{
    typedef std::pair<IInternalRenderObject*, bool> Object;
    AABB m_box;
    Octree* m_children[8];
    std::vector<Object*> m_containigObjects;
    Frustum* m_frustum;
    bool m_isLeaf, m_isRoot;
    void SetFrustum(Frustum* frustum);
    void AddObject(Object *object);
    void ResetObjects();
    Octree(float left, float right, float bottom, float top, float front, float back, bool isRoot);
public:
    Octree(float left, float right, float bottom, float top, float front, float back);
    void show_visible(MatInterface::MaterialInterface &);
    void Subdivide(int iterations);
#ifdef RENDER_OCTREE
    void PrepareToRender(ID3D11Device*);
#endif
    void add_object(IInternalRenderObject *object);
    void Shutdown();
    ~Octree();
};

