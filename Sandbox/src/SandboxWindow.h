#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace SandboxWindow
{

    class SandboxWindow
    {
    private:
        GLFWwindow *_window;
        GLint _x;
        GLint _y;
        GLsizei _width;
        GLsizei _height;

    public:
        SandboxWindow(int width, int height, const char *title);
        ~SandboxWindow();

        void refresh();
        bool isClose();
        /**
         * 初始化GLAD
         * 我们给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。
         * GLFW给我们的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数。
         */
        void initGLAD();
        /**
         * 视口
         * 我们必须告诉OpenGL渲染窗口的尺寸大小
         * glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）
         */
        void setViewport();
        void setViewport(GLint x, GLint y, GLsizei width, GLsizei height);

        void processInput();

        void onMouse(void mouse_callback(GLFWwindow *window, double xPos, double yPos));

        /**
         * 当用户改变窗口的大小的时候，视口也应该被调整
         */
        void onResize(void framebuffer_size_callback(GLFWwindow *window, int width, int height));
        void autoResize();

        GLFWwindow * getWindow() { return _window; }
    };

} // namespace SandboxWindow
