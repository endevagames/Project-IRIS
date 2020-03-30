#ifndef CELL_RESOURCES_MESH_LOADER_H
#define CELL_RESOURCES_MESH_LOADER_H

#include <string>
#include <vector>

#include "Math/Linear/Vector.h"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
struct aiString;

namespace NURU
{
    class Renderer;
    class SceneNode;
    class Mesh;
    class Material;

    /* 

      Mesh load functionality.

    */
    class MeshLoader
    {
    private:
        // NOTE(Joey): keep track of all loaded mesh
        static std::vector<Mesh*> meshStore;
    public:
        static void       Clean();
        static SceneNode* LoadMesh(Renderer* renderer, std::string path, bool setDefaultMaterial = true);
    private:
        static SceneNode* processNode(Renderer* renderer, aiNode* aNode, const aiScene* aScene, std::string directory, bool setDefaultMaterial);
        static Mesh*      parseMesh(aiMesh* aMesh, const aiScene* aScene, Vec3& out_Min, Vec3& out_Max);
        static Material*  parseMaterial(Renderer* renderer, aiMaterial* aMaterial, const aiScene* aScene, std::string directory);

        static std::string processPath(aiString* path, std::string directory);
    };
}
#endif