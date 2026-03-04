#include "Shader.h"

#include <iostream>
#include <fstream>

static GLuint      CreateShader(const std::string& text, GLenum shaderType);
static void        CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errnoMessage);
static std::string LoadShader(const std::string& filename);

GLuint CreateShader(const std::string& text, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0)
    {
        std::cerr << "Error: Shader creation failed!" << std::endl;
    }
    const GLchar* shaderSourceStrings[1];
    GLint         shaderSourceStringLengths[1];
    shaderSourceStrings[0]       = text.c_str();
    shaderSourceStringLengths[0] = static_cast<GLint>(text.length());
    glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
    glCompileShader(shader);
    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed!");
    return shader;
}

void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errnoMessage)
{
    GLint  success     = 0;
    GLchar error[1024] = { 0 };

    if (isProgram)
    {
        glGetProgramiv(shader, flag, &success);
    }
    else
    {
        glGetShaderiv(shader, flag, &success);
    }

    if (success == GL_FALSE)
    {
        if (isProgram)
        {
            glGetProgramInfoLog(shader, sizeof(error), nullptr, error);
        }
        else
        {
            glGetShaderInfoLog(shader, sizeof(error), nullptr, error);
        }
        std::cerr << errnoMessage << ": '" << error << "'" << std::endl;
    }
}

std::string LoadShader(const std::string& filename)
{
    std::ifstream file;
    file.open(filename);

    std::string output;
    std::string line;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            output += line + "\n";
        }
    }
    else
    {
        std::cerr << "无法打开文件: " << filename << std::endl;
    }

    return output;
}

Shader::Shader(const std::string& filename)
{
    program_ = glCreateProgram();

    shaders_[0] = CreateShader(LoadShader(filename + ".vs"), GL_VERTEX_SHADER);
    shaders_[1] = CreateShader(LoadShader(filename + ".fs"), GL_FRAGMENT_SHADER);

    for (unsigned int shader : shaders_)
    {
        glAttachShader(program_, shader);
    }

    glBindAttribLocation(program_, 0, "aPos");
    glBindAttribLocation(program_, 1, "aTexCoord");

    /// 链接程序
    glLinkProgram(program_);
    CheckShaderError(program_, GL_LINK_STATUS, true, "Error: Program linking failed!");

    /// 验证程序
    glValidateProgram(program_);
    CheckShaderError(program_, GL_VALIDATE_STATUS, true, "Error: Program is invalid!");


    // GLint transformLoc = glGetUniformLocation(program_, "transform");
    // std::cout << "Transform uniform location: " << transformLoc << std::endl;
    // uniforms_[TRANSFORM_U] = transformLoc;

    uniforms_[TRANSFORM_U] = glGetUniformLocation(program_, "transform");
}

Shader::~Shader()
{
    for (unsigned int shader : shaders_)
    {
        glDetachShader(program_, shader);
        glDeleteShader(shader);
    }
    glDeleteProgram(program_);
}

void Shader::bind()
{
    glUseProgram(program_);
}

void Shader::update(const Transform& transform)
{
    glm::mat4 model = transform.getModel();
    // std::cout << "Updating transform, matrix[0][0] = " << model[0][0] << std::endl; // 看值是否变化
    glUniformMatrix4fv(uniforms_[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
}
