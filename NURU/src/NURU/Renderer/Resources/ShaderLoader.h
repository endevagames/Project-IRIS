#pragma once

#include <string>
#include <fstream>

#include "../shading/shader.h"

namespace NURU
{
    /* 

      A static helper class that does the relevant file IO and custom shader pre-processing to load 
      and parse shader code.  

    */
    class ShaderLoader
    {
    public:
        static Shader Load(std::string name, std::string vsPath, std::string fsPath, std::vector<std::string> defines = std::vector<std::string>());
    private:
        static std::string readShader(std::ifstream& file, const std::string& name, std::string path);
    };
}