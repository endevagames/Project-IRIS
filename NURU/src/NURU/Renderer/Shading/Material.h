#pragma once

#include "ShadingTypes.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureCube.h"

#include "Math/Linear/Vector.h"

#include <glad/glad.h>

#include <map>


namespace NURU
{
    enum MaterialType
    {
        MATERIAL_DEFAULT,
        MATERIAL_CUSTOM,
        MATERIAL_POST_PROCESS,
    };

    // TODO(Joey): should be able to copy materials.
    // TODO(Joey): should not be able to change the shader of the material; this is set during creation via the renderer.
    /* 

      Material object, representing all render state required for rendering a mesh. This includes 
      shader parameters, texture samplers and GL state relevant for rendering a mesh. A material 
      object is required for rendering any scene node. The renderer holds a list of common material
      defaults/templates for deriving or creating new materials.

    */
    class Material
    {
    private:
        // shader state
        Shader* m_Shader;
        std::map<std::string, UniformValue>        m_Uniforms;
        std::map<std::string, UniformValueSampler> m_SamplerUniforms; // NOTE(Joey): process samplers differently 
    public:
        MaterialType Type = MATERIAL_CUSTOM;
        Vec4 Color  = Vec4(1.0f);

        // depth state
        bool   DepthTest    = true;
        bool   DepthWrite   = true;
        GLenum DepthCompare = GL_LESS;

        // face culling state
        bool   Cull             = true;
        GLenum CullFace         = GL_BACK;
        GLenum CullWindingOrder = GL_CCW;

        // blending state
        bool   Blend         = false;
        GLenum BlendSrc      = GL_ONE; // pre-multiplied alpha
        GLenum BlendDst      = GL_ONE_MINUS_SRC_ALPHA;
        GLenum BlendEquation = GL_FUNC_ADD;

        // shadow state
        bool ShadowCast    = true;
        bool ShadowReceive = true;

    private:
      
    public:
        Material();
        Material(Shader* shader);

        Shader* GetShader();
        void    SetShader(Shader* shader);

        Material Copy();

        void SetBool(std::string name,        bool value);
        void SetInt(std::string name,         int value);
        void SetFloat(std::string name,       float value);
        void SetTexture(std::string name,     Texture* value, unsigned int unit = 0);
        void SetTextureCube(std::string name, TextureCube* value, unsigned int unit = 0);
        void SetVector(std::string name,      Vec2 value);
        void SetVector(std::string name,      Vec3 value);
        void SetVector(std::string name,      Vec4 value);
        void SetMatrix(std::string name,      Mat2 value);
        void SetMatrix(std::string name,      Mat3 value);
        void SetMatrix(std::string name,      Mat4 value);

        std::map<std::string, UniformValue>*        GetUniforms();
        std::map<std::string, UniformValueSampler>* GetSamplerUniforms();
    };
}