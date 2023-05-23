#include <iostream>
#include "SandboxWindow.h"

namespace SandboxWindow
{
    SandboxWindow::SandboxWindow(int width, int height, const char *title) : _x(0), _y(0)
    {
        glfwInit();
        /** 设定OpenGL 版本: 3.3 */
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 指定OpenGL主版本
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 指定OpenGL次要版本
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        /** MacOS 需要下面这行 */
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        _width = width;
        _height = height;
        /** 创建一个窗口对象 */
        _window = glfwCreateWindow(_width, _height, title, NULL, NULL);
        if (_window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            throw -1;
        }
        glfwMakeContextCurrent(_window);
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    SandboxWindow::~SandboxWindow()
    {
        /**
         * 释放/删除之前的分配的所有资源
         */
        glfwDestroyWindow(_window);
        glfwTerminate();
    }

    void SandboxWindow::refresh()
    {
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    bool SandboxWindow::isClose()
    {
        return glfwWindowShouldClose(_window);
    }

    void SandboxWindow::initGLAD()
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            throw -1;
        }
    }

    void SandboxWindow::setViewport()
    {
        glViewport(_x, _y, _width, _height);
    }
    void SandboxWindow::setViewport(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        _x = x;
        _y = y;
        _width = width;
        _height = height;
        glViewport(_x, _y, _width, _height);
    }

    void SandboxWindow::processInput()
    {
        if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(_window, true);
    }

    void SandboxWindow::onMouse(void mouse_callback(GLFWwindow *window, double xPos, double yPos))
    {
        // glfwSetCursorPosCallback(_window, mouse_callback);
    }

    void SandboxWindow::onResize(void framebuffer_size_callback(GLFWwindow *window, int width, int height))
    {
        glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    }

    void SandboxWindow::autoResize()
    {
        glfwSetFramebufferSizeCallback(_window, [](GLFWwindow *window, int width, int height)
                                       {
        auto self = static_cast<SandboxWindow*>(glfwGetWindowUserPointer(window));
        self->setViewport(0,0, width, height); });
    }

} // namespace SandboxWindow
