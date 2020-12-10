#ifndef SHADER_HPP
#define SHADER_HPP

struct Shader
{
    GLuint program;
};

// SHOULD NOT BE USED DIRECTLY -- USE THE ASSET MANAGER INSTEAD!!!
INTERNAL void LoadShader (Shader& shader, std::string file_name);
INTERNAL void FreeShader (Shader& shader);

#endif /* SHADER_HPP */
