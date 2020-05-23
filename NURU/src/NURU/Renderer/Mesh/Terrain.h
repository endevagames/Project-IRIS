#pragma once

#include "NURU/Core/Core.h"
#include "NURU/Renderer/Mesh/Plane.h"
#include "NURU/Renderer/Shading/Texture.h"

namespace NURU
{
    class Terrain : public Mesh
    {
    private:
        struct NoiseMap
        {
			Texture* m_Texture;
            unsigned int Width;
            unsigned int Height;
            float** Map;

            NoiseMap(const unsigned int width, const unsigned int height);
            ~NoiseMap();
            void GenerateNoise(unsigned int seed, float scale, float persistence, float lacunarity, Vec2 offset, unsigned int octaves);

            float* operator [](int i) const {return Map[i];}
            float* & operator [](int i) {return Map[i];}
        };
    public:
        Terrain(unsigned int size = 32);
        inline Texture* GetTexture() { return m_NoiseMap->m_Texture; }
    private:
        Scope<NoiseMap> m_NoiseMap;
    };
}