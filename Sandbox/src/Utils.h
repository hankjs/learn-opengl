#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#define ASSERT(x) if (!(x)) __debugbreak()

#define GLCall(x) do { \
    Utils::GLClearError();\
    x;\
    ASSERT(Utils::GLLogCall(#x, __FILE__, __LINE__));\
} while (0)

namespace Utils
{

    void GLClearError();

    bool GLLogCall(const char* function, const char* file, int line);
} // namespace Utils
