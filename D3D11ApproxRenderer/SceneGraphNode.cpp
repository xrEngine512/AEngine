#include "SceneGraphNode.h"
#include <ApproxSystemTools.h>
#include "MaterialInterface.h"

#ifdef RENDER_OCTREE
#include "RenderObject.h"
#endif

using namespace MatInterface;

const float SCREEN_DEPTH = 1000.0f;

void SceneGraphNode::SetFrustum(Frustum* frustum)
{
    if (m_isLeaf)
        m_frustum = frustum;
    else
    {
        for (SceneGraphNode* node : m_children)
        {
            node->SetFrustum(frustum);
        }
    }
}

void SceneGraphNode::AddObject(Object *object)
{
    if (m_isLeaf)
    {
        m_containigObjects.push_back(object);
    }
    else
    {
        for (auto child : m_children)
        {
            if (object->first->GetMeshBox().Intersects(&child->m_box))
            {
                child->AddObject(object);
            }
        }
    }
}

void SceneGraphNode::ResetObjects()
{
    if (m_isLeaf)
    {
        for (auto object : m_containigObjects)
        {
            object->second = true;
        }
    }
    else
    {
        for (auto child : m_children)
        {
            child->ResetObjects();
        }
    }
}

SceneGraphNode::SceneGraphNode(float left, float right, float bottom, float top, float front, float back, bool isRoot) : SceneGraphNode(left, right, bottom, top, front, back)
{
    m_isRoot = false;
}

SceneGraphNode::SceneGraphNode(float left, float right, float bottom, float top, float front, float back) :
m_isLeaf(true), m_isRoot(true)
{
    m_box.left      = left;
    m_box.right     = right;
    m_box.bottom    = bottom;
    m_box.top       = top;
    m_box.front     = front;
    m_box.back      = back;
    for (SceneGraphNode* node: m_children)
    {
        node = nullptr;
    }
    m_frustum = new Frustum;
}

void SceneGraphNode::Render(ID3D11DeviceContext* context, MaterialInterface& mi)
{
    if (m_isRoot)
    {        
        m_frustum->ConstructFrustum(SCREEN_DEPTH, mi.sceneConstants.projectionMatrix.Value(), mi.perFrameData.viewMatrix.Value());
        SetFrustum(m_frustum);
        for (auto child : m_children)
        {
            child->Render(context, mi);
        }
        for (auto child : m_children)
        {
            child->ResetObjects();
        }        
    }
    else
    {
        if (m_isLeaf)
        {
#ifdef RENDER_OCTREE
            if (m_thisNode)
                m_thisNode->Render(context, mi);
#endif
            for (auto object : m_containigObjects)
            {
                if (object->second)
                {
                    if (m_frustum->CheckBox(&object->first->GetMeshBox()))
                    {               
                        object->first->Render(context, mi);
                    }
                    object->second = false;
                }
            }
        }
        else
        {
            for (auto child : m_children)
            {
                child->Render(context, mi);
            }
        }
    }
}

void SceneGraphNode::Subdivide(int iterations)
{
    if (m_isLeaf && iterations > 0)
    {
        float left = m_box.left;
        float right = m_box.right;
        float bottom = m_box.bottom;
        float top = m_box.top;
        float front = m_box.front;
        float back = m_box.back;

        XMFLOAT3 center = m_box.GetCenter();
        
        /*
        0   0   0
        0   0   1
        0   1   0
        0   1   1
        1   0   0
        1   0   1
        1   1   0
        1   1   1
        */


        m_children[0] = new SceneGraphNode(left, center.x, bottom, center.y, front, center.z, false);
        m_children[1] = new SceneGraphNode(left, center.x, bottom, center.y, center.z, back, false);
        m_children[2] = new SceneGraphNode(left, center.x, center.y, top, front, center.z, false);
        m_children[3] = new SceneGraphNode(left, center.x, center.y, top, center.z, back, false);
        m_children[4] = new SceneGraphNode(center.x, right, bottom, center.y, front, center.z, false);
        m_children[5] = new SceneGraphNode(center.x, right, bottom, center.y, center.z, back, false);
        m_children[6] = new SceneGraphNode(center.x, right, center.y, top, front, center.z, false);
        m_children[7] = new SceneGraphNode(center.x, right, center.y, top, center.z, back, false);
        iterations--;

        for (auto child : m_children)
        {
            child->Subdivide(iterations);
        }

        for (auto object : m_containigObjects)
        {            
            for (auto child : m_children)
            {
                if (object->first->GetMeshBox().Intersects(&child->m_box))
                {
                    child->AddObject(object);
                }
            }
        }
        m_containigObjects.clear();
        m_isLeaf = false;
    }
}

#ifdef RENDER_OCTREE
void SceneGraphNode::PrepareToRender(ID3D11Device* device)
{
    if (m_isLeaf)
    {
        m_thisNode = new RenderObject(device, WIREFRAME_SHADER);
        m_thisNode->Initialize(device, &m_box, 0, 0, 0, 0, 0,0);
    }
    else
    {
        for (auto child : m_children)
        {
            child->PrepareToRender(device);
        }
    }
}
#endif

void SceneGraphNode::AddObject(IInternalRenderObject* object)
{
    if (m_isLeaf)
    {
        Object* obj = new Object(object, true);
        m_containigObjects.push_back(obj);
    }
    else
    {
        if (m_isRoot)
        {
            Object *obj = new Object(object, true);
            for (auto child : m_children)
            {
                if (object->GetMeshBox().Intersects(&child->m_box))
                {
                    child->AddObject(obj);
                }
            }
        }
    }
}

void SceneGraphNode::Shutdown()
{
    if (m_isLeaf)
    {
        for (auto object : m_containigObjects)
        {
            DELETE_SYS_OBJECT(object->first)
        }
    }
    else
    {
        for (auto child : m_children)
        {
            child->Shutdown();
        }
    }
    if (m_isRoot)
    {
        delete m_frustum;
    }
}

SceneGraphNode::~SceneGraphNode()
{
}
