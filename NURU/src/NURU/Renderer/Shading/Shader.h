#pragma once

#include <string>
#include <vector>

#include "Math/Linear/Vector.h"
#include "Math/Linear/Matrix.h"

#include "ShadingTypes.h"

namespace NURU
{
    /* 

      Shader object for quickly creating and using a GPU shader object. When compiling/linking a 
      shader object from source code, all vertex attributes and uniforms are extracted for saving
      unnecessary additional CPU->GPU roundtrip times.

    */
    class Shader
    {
    public:
        unsigned int ID;
        std::string  Name;

        std::vector<Uniform>         Uniforms;
        std::vector<VertexAttribute> Attributes;

    public:
        Shader();
        Shader(std::string name, std::string vsCode, std::string fsCode, std::vector<std::string> defines = std::vector<std::string>());

        void Load(std::string name, std::string vsCode, std::string fsCode, std::vector<std::string> defines = std::vector<std::string>());

        void Use();

        bool HasUniform(std::string name);

        void SetInt   (std::string location, int   value);
        void SetBool  (std::string location, bool  value);
        void SetFloat (std::string location, float value);
        void SetVector(std::string location, Vec2  value);
        void SetVector(std::string location, Vec3  value);
        void SetVector(std::string location, Vec4  value);
        void SetVectorArray(std::string location, int size, const std::vector<Vec2>& values);
        void SetVectorArray(std::string location, int size, const std::vector<Vec3>& values);
        void SetVectorArray(std::string location, int size, const std::vector<Vec4>& values);
        void SetMatrix(std::string location, Mat2 value);
        void SetMatrix(std::string location, Mat3 value);
        void SetMatrix(std::string location, Mat4 value);
        void SetMatrixArray(std::string location, int size, Mat2* values);
        void SetMatrixArray(std::string location, int size, Mat3* values);
        void SetMatrixArray(std::string location, int size, Mat4* values);
    private:
        // retrieves uniform location from pre-stored uniform locations and reports an error if a 
        // non-uniform is set.
        int getUniformLocation(std::string name);
    };
}