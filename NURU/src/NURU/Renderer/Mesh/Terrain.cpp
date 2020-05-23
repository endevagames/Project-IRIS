#include "NURUPCH.h"

#include "Terrain.h"

#include "NURU/Core/Math/Linear/Operation.h"
#include "NURU/Core/Math/Noise/PerlinNoise.hpp"
#include "NURU/Renderer/Shading/Texture.h"

#include <random>
#include <time.h> 

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <math.h>

float InverseLerp(float xx, float yy, float value)
{
    return (value - xx)/(yy - xx);
}

namespace NURU
{
    Terrain :: Terrain(unsigned int size)
    {
        unsigned int xSegments = (size * 16) / 16;
        unsigned int ySegments = (size * 16) / 16;

        srand(time(NULL));
        m_NoiseMap = CreateScope<NoiseMap>(xSegments + 1, ySegments + 1);
        m_NoiseMap->GenerateNoise(rand() % 9999, 25, 0.015, 0.52, Vec2(0.4), 4);

        float dX = (float)size  / xSegments;
        float dY = (float)size / ySegments;

        for (unsigned int y = 0; y <= ySegments; ++y)
        {
            for (unsigned int x = 0; x <= xSegments; ++x)
            {
                auto height = (*m_NoiseMap)[y][x];
                auto position = Vec3(dX * x * 2.0f - 1.0f, height, dY * y * 2.0f - 1.0f);

                Positions.push_back(position);
                UV.push_back(Vec2(dX * x, 1.0f - y * dY));

                float centerR, centerL, centerU, centerD = height;
                
                if (y > 0)
                    centerU = (*m_NoiseMap)[y - 1][x];

                if ( x > 0)
                    centerL = (*m_NoiseMap)[y][x - 1];

                if ( y < ySegments)
                    centerD = (*m_NoiseMap)[y + 1][x];

                if (x < xSegments)
                    centerR = (*m_NoiseMap)[y][x + 1];

                Normals.push_back(Normalize(Vec3((centerR - centerL) / (2 * size), (centerD - centerU) /  (2 * size), -1)));
            }
        }

		//Smooth out the normals
		const float FALLOUT_RATIO = 0.5f;
		for(int z = 0; z <= ySegments + 1; z++) 
        {
			for(int x = 0; x <= xSegments + 1; x++) 
            {
				Vec3 sum = Normals[z * xSegments + x];
					
				// if (x > 0)
				// 	sum = sum + Normals[z * xSegments + x - 1] * FALLOUT_RATIO;

				// if (x < xSegments)
				// 	sum =  sum + Normals[z * xSegments + x + 1] * FALLOUT_RATIO;

				// if (z > 0)
				// 	sum = sum + Normals[(z - 1) * xSegments + x] * FALLOUT_RATIO;

				// if (z < ySegments)
				// 	sum + sum + Normals[(z + 1) * xSegments + x] * FALLOUT_RATIO;

                // auto magnitude = sqrt(pow(sum.x, 2) + pow(sum.y, 2));
					
				// if (magnitude == 0) 
				// 	sum = Vec3(0.0f, 1.0f, 0.0f);

				// Normals[z * xSegments + x] = sum;
			}
		}

        bool oddRow = false;
        
        for (int y = 0; y < ySegments; ++y)
        {
            if (!oddRow)
            {
                for (int x = 0; x <= xSegments; ++x)
                {
                    Indices.push_back(y       * (xSegments + 1) + x);
                    Indices.push_back((y + 1) * (xSegments + 1) + x);
                }
            }
            else
            {
                for (int x = xSegments; x >= 0; --x)
                {
                    Indices.push_back((y + 1) * (xSegments + 1) + x);
                    Indices.push_back(y       * (xSegments + 1) + x);
                }
            }
            oddRow = !oddRow;
        }

        Topology = TRIANGLE_STRIP;
        Finalize();
    }
    
    Terrain :: NoiseMap :: NoiseMap(unsigned int width, unsigned int height) : Width(width), Height(height)
    {
        m_Texture = new Texture();
        Map = new float * [Width];
        for (unsigned int i = 0; i < Width; i++) Map[i] = new float[Height];
    }

    Terrain :: NoiseMap :: ~NoiseMap()
    {
        for (unsigned int j = 0; j < Width; j++)
            free(Map[j]);
        free(Map);
        delete m_Texture;
    }

    void Terrain :: NoiseMap :: GenerateNoise(unsigned int seed, float scale, float persistence, float lacunarity, Vec2 offset, unsigned int octaves)
    {
        auto noiseGen = PerlinNoise(seed);
        
        for (unsigned int j = 0; j < Height; j++)
        {
            for (unsigned int i = 0; i < Width; i++)
            {
                Map[i][j] = (float)noiseGen.accumulatedOctaveNoise2D((double)i / 16.0f, (double)j / 16.0f, 6) * 8.0f;
            }
        }
    }
}