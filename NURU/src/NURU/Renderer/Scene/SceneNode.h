#pragma once

#include <vector>

#include "Math/Linear/Vector.h"
#include "Math/Linear/Matrix.h"

namespace NURU
{
    class Scene;
    class Mesh;
    class Material;

    /* NOTE(Joey):

      An individual scene node that links a mesh to a material
      to render the mesh with while at the same time
      maintaining a parent-child hierarchy for transform
      relations.

      Each node can have any number of children (via a linked
      list) and represents a single renderable entity in a
      larger scene where aech child transform on top of their
      parent node.

    */
    class SceneNode
    {
    public:
        // each node contains relevant render state
        Mesh*     Mesh = NULL;
        Material* Material = NULL;

        // bounding box 
        Vec3 BoxMin = Vec3(-99999.0f);
        Vec3 BoxMax = Vec3( 99999.0f);
    private:
        std::vector<SceneNode*> m_Children;
        SceneNode* m_Parent = NULL;

        // per-node transform (w/ parent-child relationship)
        Mat4 m_Transform;
        Mat4 m_PrevTransform;
        Vec3 m_Position = Vec3(0.0f);
        Vec4 m_Rotation; // axis-angle for now; test w/ quaternions soon!
        Vec3 m_Scale = Vec3(1.0f);

        // mark the current node's tranform as dirty if it needs to be re-calculated this frame
        bool m_Dirty;

        // each node is uniquely identified by a 32-bit incrementing unsigned integer
        unsigned int m_ID;

        static unsigned int CounterID;
    public:
        SceneNode(unsigned int id);
        ~SceneNode();

        // scene node transform
        void SetPosition(Vec3 position);
        void SetRotation(Vec4 rotation);
        void SetScale(Vec3 scale);
        void SetScale(float scale);
        Vec3 GetLocalPosition();
        Vec4 GetLocalRotation();
        Vec3 GetLocalScale();
        Vec3 GetWorldPosition();
        Vec3 GetWorldScale();

        // scene graph 
        unsigned int GetID();
        void AddChild(SceneNode *node);
        void RemoveChild(unsigned int id); // TODO(Joey): think of proper way to unqiuely idetnfiy child nodes (w/ incrementing node ID or stringed hash ID?)
        std::vector<SceneNode*> GetChildren();
        unsigned int            GetChildCount();
        SceneNode              *GetChild(unsigned int id);
        SceneNode              *GetChildByIndex(unsigned int index);
        SceneNode              *GetParent();

        // returns the transform of the current node combined with its parent(s)' transform.
        Mat4 GetTransform();
        Mat4 GetPrevTransform();

        // re-calculates this node and its children's transform components if its parent or the 
        // node itself is dirty.
        void UpdateTransform(bool updatePrevTransform = false);
    };
}