#pragma once

#include <functional>

#include "NURU/Core/Core.h"

#include "Math/Linear/Matrix.h"
#include "Math/Linear/Vector.h"


namespace NURU
{
    /*

      Manually define a list of topology types as we don't want to directly link a mesh to an 
      OpenGL toplogy type as this would reduce the renderer's cross compatbility.

      Yes, yes I know that we still have OpenGL indices in here (VAO, VBO, EBO) which we'll 
      get rid of in a cross renderer way soon enough.

    */
    enum TOPOLOGY
    {
        POINTS,
        LINES,
        LINE_STRIP,
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,
    };

    /* 

      Base Mesh class. A mesh in its simplest form is purely a list of vertices, with some added 
      functionality for easily setting up the hardware configuration relevant for rendering.

    */
    class Mesh
    {
        // NOTE(Joey): public for now for testing and easy access; will eventually be private and only visible to renderer (as a friend class)
    public:
        unsigned int m_VAO = 0;
        unsigned int m_VBO;
        unsigned int m_EBO;
    public:
        List<Vec3> Positions;
        List<Vec2> UV;
        List<Vec3> Normals;
        List<Vec3> Tangents;
        List<Vec3> Bitangents;

        TOPOLOGY Topology = TRIANGLES;
        List<unsigned int> Indices;

        // support multiple ways of initializing a mesh
        Mesh();
        Mesh(List<Vec3> positions, List<unsigned int> indices);
        Mesh(List<Vec3> positions, List<Vec2> uv, List<unsigned int> indices);
        Mesh(List<Vec3> positions, List<Vec2> uv, List<Vec3> normals, List<unsigned int> indices);
        Mesh(List<Vec3> positions, List<Vec2> uv, List<Vec3> normals, List<Vec3> tangents, List<Vec3> bitangents, List<unsigned int> indices);

        // set vertex data manually
        // TODO(Joey): not sure if these are required if we can directly set vertex data from public fields; construct several use-cases to test.
        void SetPositions(List<Vec3> positions);
        void SetUVs(List<Vec2> uv);
        void SetNormals(List<Vec3> normals);
        void SetTangents(List<Vec3> tangents, List<Vec3> bitangents); // NOTE(Joey): you can only set both tangents and bitangents at the same time to prevent mismatches

        // commits all buffers and attributes to the GPU driver
        void Finalize(bool interleaved = true);

        // generate triangulated mesh from signed distance field
        void FromSDF(std::function<float(Vec3)>& sdf, float maxDistance, uint16_t gridResolution);

    private:
        void CalculateNormals(bool smooth = true);
        void CalculateTangents();
    };
}