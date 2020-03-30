#pragma once

#include "Math/Linear/Vector.h"
#include "Math/Linear/Matrix.h"

#include <string>

namespace NURU
{
    class Texture;
    class TextureCube;

    enum SHADER_TYPE
    {
        SHADER_TYPE_BOOL,
        SHADER_TYPE_INT,
        SHADER_TYPE_FLOAT,
        SHADER_TYPE_SAMPLER1D,
        SHADER_TYPE_SAMPLER2D,
        SHADER_TYPE_SAMPLER3D,
        SHADER_TYPE_SAMPLERCUBE,
        SHADER_TYPE_VEC2,
        SHADER_TYPE_VEC3,
        SHADER_TYPE_VEC4,
        SHADER_TYPE_MAT2,
        SHADER_TYPE_MAT3,
        SHADER_TYPE_MAT4,
    };

    struct Uniform
    {
        SHADER_TYPE  Type;
        std::string  Name;
        int          Size;
        unsigned int Location;
    };

    struct UniformValue
    {
        SHADER_TYPE Type;
        // TODO(Joey): now each element takes up the space of its largest 
        // element (mat4) which is 64 bytes; come up with a better solution!
        union
        {
            bool       Bool;
            int        Int;
            float      Float;

            Vec2 Vec2;
            Vec3 Vec3;
            Vec4 Vec4;
            Mat2 Mat2;
            Mat3 Mat3;
            Mat4 Mat4;
        };

        UniformValue() {}
    };

    struct UniformValueSampler
    {
        SHADER_TYPE  Type;
        unsigned int Unit;
        union 
        {
            Texture     *Texture;
            TextureCube *TextureCube;
        };

        UniformValueSampler() {}
    };

    struct VertexAttribute
    {
        SHADER_TYPE  Type;
        std::string  Name;
        int          Size;
        unsigned int Location;
    };
}