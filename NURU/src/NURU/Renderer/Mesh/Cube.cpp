#include "NURUPCH.h"

#include "Cube.h"

namespace NURU
{
    // --------------------------------------------------------------------------------------------
    Cube::Cube()
    {
        Positions = std::vector<Vec3> {
            Vec3(-0.5f, -0.5f, -0.5f),
            Vec3( 0.5f,  0.5f, -0.5f), 
            Vec3( 0.5f, -0.5f, -0.5f), 
            Vec3( 0.5f,  0.5f, -0.5f), 
            Vec3(-0.5f, -0.5f, -0.5f),
            Vec3(-0.5f,  0.5f, -0.5f),

            Vec3(-0.5f, -0.5f,  0.5f),
            Vec3( 0.5f, -0.5f,  0.5f),
            Vec3( 0.5f,  0.5f,  0.5f),
            Vec3( 0.5f,  0.5f,  0.5f), 
            Vec3(-0.5f,  0.5f,  0.5f),
            Vec3(-0.5f, -0.5f,  0.5f),

            Vec3(-0.5f,  0.5f,  0.5f),
            Vec3(-0.5f,  0.5f, -0.5f),
            Vec3(-0.5f, -0.5f, -0.5f),
            Vec3(-0.5f, -0.5f, -0.5f),
            Vec3(-0.5f, -0.5f,  0.5f),
            Vec3(-0.5f,  0.5f,  0.5f),

            Vec3( 0.5f,  0.5f,  0.5f), 
            Vec3( 0.5f, -0.5f, -0.5f), 
            Vec3( 0.5f,  0.5f, -0.5f), 
            Vec3( 0.5f, -0.5f, -0.5f), 
            Vec3( 0.5f,  0.5f,  0.5f), 
            Vec3( 0.5f, -0.5f,  0.5f), 

            Vec3(-0.5f, -0.5f, -0.5f),
            Vec3( 0.5f, -0.5f, -0.5f),
            Vec3( 0.5f, -0.5f,  0.5f),
            Vec3( 0.5f, -0.5f,  0.5f), 
            Vec3(-0.5f, -0.5f,  0.5f),
            Vec3(-0.5f, -0.5f, -0.5f),

            Vec3(-0.5f,  0.5f, -0.5f),
            Vec3( 0.5f,  0.5f,  0.5f), 
            Vec3( 0.5f,  0.5f, -0.5f), 
            Vec3( 0.5f,  0.5f,  0.5f), 
            Vec3(-0.5f,  0.5f, -0.5f),
            Vec3(-0.5f,  0.5f,  0.5f),
        };
        UV = std::vector<Vec2> {
            Vec2(0.0f, 0.0f),
            Vec2(1.0f, 1.0f),
            Vec2(1.0f, 0.0f),
            Vec2(1.0f, 1.0f),
            Vec2(0.0f, 0.0f),
            Vec2(0.0f, 1.0f),

            Vec2(0.0f, 0.0f),
            Vec2(1.0f, 0.0f),
            Vec2(1.0f, 1.0f),
            Vec2(1.0f, 1.0f),
            Vec2(0.0f, 1.0f),
            Vec2(0.0f, 0.0f),

            Vec2(1.0f, 0.0f),
            Vec2(1.0f, 1.0f),
            Vec2(0.0f, 1.0f),
            Vec2(0.0f, 1.0f),
            Vec2(0.0f, 0.0f),
            Vec2(1.0f, 0.0f),

            Vec2(1.0f, 0.0f),
            Vec2(0.0f, 1.0f),
            Vec2(1.0f, 1.0f),
            Vec2(0.0f, 1.0f),
            Vec2(1.0f, 0.0f),
            Vec2(0.0f, 0.0f),

            Vec2(0.0f, 1.0f),
            Vec2(1.0f, 1.0f),
            Vec2(1.0f, 0.0f),
            Vec2(1.0f, 0.0f),
            Vec2(0.0f, 0.0f),
            Vec2(0.0f, 1.0f),

            Vec2(0.0f, 1.0f),
            Vec2(1.0f, 0.0f),
            Vec2(1.0f, 1.0f),
            Vec2(1.0f, 0.0f),
            Vec2(0.0f, 1.0f),
            Vec2(0.0f, 0.0f),
        };
        Normals = std::vector<Vec3>{
            Vec3( 0.0f,  0.0f, -1.0f),
            Vec3( 0.0f,  0.0f, -1.0f),
            Vec3( 0.0f,  0.0f, -1.0f),
            Vec3( 0.0f,  0.0f, -1.0f),
            Vec3( 0.0f,  0.0f, -1.0f),
            Vec3( 0.0f,  0.0f, -1.0f),

            Vec3( 0.0f,  0.0f,  1.0f),
            Vec3( 0.0f,  0.0f,  1.0f),
            Vec3( 0.0f,  0.0f,  1.0f),
            Vec3( 0.0f,  0.0f,  1.0f),
            Vec3( 0.0f,  0.0f,  1.0f),
            Vec3( 0.0f,  0.0f,  1.0f),

            Vec3(-1.0f,  0.0f,  0.0f),
            Vec3(-1.0f,  0.0f,  0.0f),
            Vec3(-1.0f,  0.0f,  0.0f),
            Vec3(-1.0f,  0.0f,  0.0f),
            Vec3(-1.0f,  0.0f,  0.0f),
            Vec3(-1.0f,  0.0f,  0.0f),

            Vec3( 1.0f,  0.0f,  0.0f),
            Vec3( 1.0f,  0.0f,  0.0f),
            Vec3( 1.0f,  0.0f,  0.0f),
            Vec3( 1.0f,  0.0f,  0.0f),
            Vec3( 1.0f,  0.0f,  0.0f),
            Vec3( 1.0f,  0.0f,  0.0f),

            Vec3( 0.0f, -1.0f,  0.0f),
            Vec3( 0.0f, -1.0f,  0.0f),
            Vec3( 0.0f, -1.0f,  0.0f),
            Vec3( 0.0f, -1.0f,  0.0f),
            Vec3( 0.0f, -1.0f,  0.0f),
            Vec3( 0.0f, -1.0f,  0.0f),

            Vec3( 0.0f,  1.0f,  0.0f),
            Vec3( 0.0f,  1.0f,  0.0f),
            Vec3( 0.0f,  1.0f,  0.0f),
            Vec3( 0.0f,  1.0f,  0.0f),
            Vec3( 0.0f,  1.0f,  0.0f),
            Vec3( 0.0f,  1.0f,  0.0f),
        };

        Topology = TRIANGLES;
        Finalize();
    }
}