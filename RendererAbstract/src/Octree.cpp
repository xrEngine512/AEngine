#include "Octree.h"
#include <ApproxSystemTools.h>
#include "MaterialInterface.h"

#ifdef RENDER_OCTREE
#include "RenderObject.h"
#endif

using namespace MatInterface;
using namespace Math;

const float SCREEN_DEPTH = 1000.0f;

void Octree::SetFrustum(Frustum* frustum)
{
    if (m_isLeaf)
        m_frustum = frustum;
    else
    {
        for (Octree* node : m_children)
        {
            node->SetFrustum(frustum);
        }
    }
}

void Octree::AddObject(Object *object)
{
    if (m_isLeaf)
    {
        m_containigObjects.push_back(object);
    }
    else
    {
        for (auto child : m_children)
        {
            if (object->first->aabb().Intersects(child->m_box))
            {
                child->AddObject(object);
            }
        }
    }
}

void Octree::ResetObjects()
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

Octree::Octree(float left, float right, float bottom, float top, float front, float back, bool isRoot) : Octree(left, right, bottom, top, front, back)
{
    m_isRoot = false;
}

Octree::Octree(float left, float right, float bottom, float top, float front, float back) :
m_isLeaf(true), m_isRoot(true)
{
    m_box.left      = left;
    m_box.right     = right;
    m_box.bottom    = bottom;
    m_box.top       = top;
    m_box.front     = front;
    m_box.back      = back;
    for (Octree* node: m_children)
    {
        node = nullptr;
    }
    m_frustum = new Frustum;
}

void Octree::show_visible(MaterialInterface &mi)
{
    if (m_isRoot)
    {        
		m_frustum->ConstructFrustum(SCREEN_DEPTH, ATranspose(mi.sceneConstants.projectionMatrix.Value()), ATranspose(mi.perFrameData.viewMatrix.Value()));
        SetFrustum(m_frustum);
        for (auto child : m_children)
        {
            child->show_visible(mi);
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
            for (auto object : m_containigObjects)
            {
                if (object->second)
                {
                    if (m_frustum->CheckBox(object->first->aabb()))
                    {               
                        object->first->set_is_visible();
                    }
                    object->second = false;
                }
            }
        }
        else
        {
            for (auto child : m_children)
            {
                child->show_visible(mi);
            }
        }
    }
}

void Octree::Subdivide(int iterations)
{
    if (m_isLeaf && iterations > 0)
    {
        float left = m_box.left;
        float right = m_box.right;
        float bottom = m_box.bottom;
        float top = m_box.top;
        float front = m_box.front;
        float back = m_box.back;

        AVector3 center = m_box.GetCenter();
        
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


        m_children[0] = new Octree(left, center.x, bottom, center.y, front, center.z, false);
        m_children[1] = new Octree(left, center.x, bottom, center.y, center.z, back, false);
        m_children[2] = new Octree(left, center.x, center.y, top, front, center.z, false);
        m_children[3] = new Octree(left, center.x, center.y, top, center.z, back, false);
        m_children[4] = new Octree(center.x, right, bottom, center.y, front, center.z, false);
        m_children[5] = new Octree(center.x, right, bottom, center.y, center.z, back, false);
        m_children[6] = new Octree(center.x, right, center.y, top, front, center.z, false);
        m_children[7] = new Octree(center.x, right, center.y, top, center.z, back, false);
        iterations--;

        for (auto child : m_children)
        {
            child->Subdivide(iterations);
        }

        for (auto object : m_containigObjects)
        {            
            for (auto child : m_children)
            {
                if (object->first->aabb().Intersects(child->m_box))
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

void Octree::add_object(IInternalRenderObject *object)
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
                if (object->aabb().Intersects(child->m_box))
                {
                    child->AddObject(obj);
                }
            }
        }
    }
}

void Octree::Shutdown()
{
    if (!m_isLeaf)
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

Octree::~Octree()
{
}
