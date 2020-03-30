#include "NURUPCH.h"

#include "Shading/Material.h"

#include "Scene.h"
#include "SceneNode.h"
#include "Mesh/Mesh.h"

#include "Math/Linear/Matrix.h"
#include "Math/Linear/Transformation.h"

#include <assert.h>

namespace NURU
{
    // --------------------------------------------------------------------------------------------
    SceneNode::SceneNode(unsigned int id) : m_ID(id)
    {
    }
    // --------------------------------------------------------------------------------------------
    SceneNode::~SceneNode()
    {
        // traverse the list of children and delete accordingly.
        for (unsigned int i = 0; i < m_Children.size(); ++i)
        {
            // it should not be possible that a scene node is childed by more than one
            // parent, thus we don't need to care about deleting dangling pointers.
            delete m_Children[i];
        }
    }
    // --------------------------------------------------------------------------------------------
    void SceneNode::SetPosition(Vec3 position)
    {
        m_Position = position;
        m_Dirty = true;
    }
    // --------------------------------------------------------------------------------------------
    void SceneNode::SetRotation(Vec4 rotation)
    {
        m_Rotation = rotation;
        m_Dirty = true;
    }
    // --------------------------------------------------------------------------------------------
    void SceneNode::SetScale(Vec3 scale)
    {
        m_Scale = scale;
        m_Dirty = true;
    }
    // --------------------------------------------------------------------------------------------
    void SceneNode::SetScale(float scale)
    {
        m_Scale = Vec3(scale);
        m_Dirty = true;
    }
    // --------------------------------------------------------------------------------------------
    Vec3 SceneNode::GetLocalPosition()
    {
        return m_Position;
    }
    // --------------------------------------------------------------------------------------------
    Vec4 SceneNode::GetLocalRotation()
    {
        return m_Rotation;
    }
    // --------------------------------------------------------------------------------------------
    Vec3 SceneNode::GetLocalScale()
    {
        return m_Scale;
    }
    // --------------------------------------------------------------------------------------------
    Vec3 SceneNode::GetWorldPosition()
    {
        Mat4 transform = GetTransform();
        Vec4 pos = transform * Vec4(m_Position, 1.0f);
        return pos.xyz;
    }
    // --------------------------------------------------------------------------------------------
    Vec3 SceneNode::GetWorldScale()
    {
        Mat4 transform = GetTransform();
        Vec3 scale = Vec3(transform[0][0], transform[1][1], transform[2][2]);
        if (scale.x < 0.0f) scale.x *= -1.0f;
        if (scale.y < 0.0f) scale.y *= -1.0f;
        if(scale.z < 0.0f) scale.z *= -1.0f;
        return scale;
    }
    // --------------------------------------------------------------------------------------------
    unsigned int SceneNode::GetID()
    {
        return m_ID;
    }
    // --------------------------------------------------------------------------------------------
    void SceneNode::AddChild(SceneNode *node)
    {
        // check if this child already has a parent. If so, first remove this scene node from its 
        // current parent. Scene nodes aren't allowed to exist under multiple parents.
        if (node->m_Parent != NULL)
        {
            node->m_Parent->RemoveChild(node->m_ID);
        }
        node->m_Parent = this;
        m_Children.push_back(node);
    }
    // --------------------------------------------------------------------------------------------
    void SceneNode::RemoveChild(unsigned int id)
    {
        auto it = std::find(m_Children.begin(), m_Children.end(), GetChild(id));
        if(it != m_Children.end())
            m_Children.erase(it);
    }
    // --------------------------------------------------------------------------------------------
    std::vector<SceneNode*> SceneNode::GetChildren()
    {
        return m_Children;
    }
    // --------------------------------------------------------------------------------------------
    unsigned int SceneNode::GetChildCount()
    {
        return m_Children.size();
    }
    // --------------------------------------------------------------------------------------------
    SceneNode *SceneNode::GetChild(unsigned int id)
    {
        for (unsigned int i = 0; i < m_Children.size(); ++i)
        {
            if(m_Children[i]->GetID() == id)
                return m_Children[i];
        }
        return nullptr;
    }
    // --------------------------------------------------------------------------------------------
    SceneNode* SceneNode::GetChildByIndex(unsigned int index)
    {
        assert(index < GetChildCount());
        return m_Children[index];
    }
    // --------------------------------------------------------------------------------------------
    SceneNode *SceneNode::GetParent()
    {
        return m_Parent;
    }
    // --------------------------------------------------------------------------------------------
    Mat4 SceneNode::GetTransform()
    {
        if (m_Dirty)
        {
            UpdateTransform(false);
        }
        return m_Transform;
    }
    // --------------------------------------------------------------------------------------------
    Mat4 SceneNode::GetPrevTransform()
    {
        return m_PrevTransform;
    }
    // --------------------------------------------------------------------------------------------
    void SceneNode::UpdateTransform(bool updatePrevTransform)
    {
        // if specified, store current transform as prev transform (for calculating motion vectors)
        if (updatePrevTransform)
        {
            m_PrevTransform = m_Transform;     
        }
        // we only do this if the node itself or its parent is flagged as dirty
        if (m_Dirty)
        {
            // first scale, then rotate, then translation
            m_Transform = NURU::Translate(m_Position);
            m_Transform = NURU::Scale(m_Transform, m_Scale); // TODO: order is off here for some reason, figure out why
			m_Transform = NURU::Rotate(m_Transform, m_Rotation.xyz, m_Rotation.w);

            if (m_Parent)
            {
                m_Transform = m_Parent->m_Transform * m_Transform;
            }        
        }
        for (int i = 0; i < m_Children.size(); ++i)
        {
            if (m_Dirty)
            {
                m_Children[i]->m_Dirty = true;
            }
            m_Children[i]->UpdateTransform(updatePrevTransform);
        }
        m_Dirty = false;
    }
}