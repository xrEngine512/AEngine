#pragma once
#include "IInternalRenderObject.h"
#include "MeshBox.h"
#include "Frustum.h"
#include <vector>
using namespace std;

namespace MatInterface
{
	class MaterialInterface;
}

class SceneGraphNode
{
    typedef pair<IInternalRenderObject*, bool> Object;
#ifdef RENDER_OCTREE
    IInternalRenderObject* m_thisNode;
#endif
    MeshBox m_box;
    SceneGraphNode* m_children[8];
    vector<Object*> m_containigObjects;
    Frustum* m_frustum;
    bool m_isLeaf, m_isRoot;
    void SetFrustum(Frustum* frustum);
    void AddObject(Object *object);
    void ResetObjects();
    SceneGraphNode(float left, float right, float bottom, float top, float front, float back, bool isRoot);
public:
    SceneGraphNode(float left, float right, float bottom, float top, float front, float back);
    void Render(ID3D11DeviceContext* context, MatInterface::MaterialInterface&);
    void Subdivide(int iterations);
#ifdef RENDER_OCTREE
    void PrepareToRender(ID3D11Device*);
#endif
    void AddObject(IInternalRenderObject* object);
    void Shutdown();
    ~SceneGraphNode();
};

