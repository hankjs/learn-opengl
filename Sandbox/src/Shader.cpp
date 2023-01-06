#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
  std::ifstream vertexFile;
  std::ifstream fragmentFile;
  std::stringstream vertexSStream;
  std::stringstream fragmentSStream;

  try
  {
    vertexFile.open(vertexPath);
    fragmentFile.open(fragmentPath);
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    if (!vertexFile.is_open() || !fragmentFile.is_open())
    {
      throw std::exception("open file error");
    }

    vertexSStream << vertexFile.rdbuf();
    fragmentSStream << fragmentFile.rdbuf();

    vertexString = vertexSStream.str();
    fragmentString = fragmentSStream.str();

    vertexSource = vertexString.c_str();
    fragmentSource = fragmentString.c_str();

    unsigned int vertex;
    unsigned int fragment;
    /**
     * 创建顶点着色器对象
     * 我们把需要创建的着色器类型以参数形式提供给`glCreateShader`。
     * 由于我们正在创建一个顶点着色器，传递的参数是`GL_VERTEX_SHADER`。
     * */
    vertex = glCreateShader(GL_VERTEX_SHADER);
    /**
     * 编译Shader
     * */
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    /**
     * 片段着色器
    */
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    /**
     * 编译Shader
     * */
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);

    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}

void Shader::use()
{
  glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
}

void Shader::setInt(const std::string &name, int value) const
{
}

void Shader::setFloat(const std::string &name, float value) const
{
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
  int success;
  char infoLog[512];

  if (type != "PROGRAM")
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
  }
  
  if (!success)
  {
    type != "PROGRAM" ? glGetShaderInfoLog(shader, 512, NULL, infoLog) : glGetProgramInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
}
