#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
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

    /** 创建一个窗口对象 */
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /**
     * 初始化GLAD
     * 我们给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。
     * GLFW给我们的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数。
     */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    {
        /** 打印OpenGL版本 */
        unsigned char *glVersion;
        glVersion = (unsigned char *)glGetString(GL_VERSION);
        std::cout << "Status: Using GL " << glVersion << std::endl;
    }

    /**
     * 视口
     * 我们必须告诉OpenGL渲染窗口的尺寸大小
     * glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）
     */
    glViewport(0, 0, 800, 600);

    /**
     * 当用户改变窗口的大小的时候，视口也应该被调整
     */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /**
     * glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序了。
     * glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
     * glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
     */
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        /**
         * 可能的缓冲位有
         * GL_COLOR_BUFFER_BIT
         * GL_DEPTH_BUFFER_BIT
         * GL_STENCIL_BUFFER_BIT
         */
        glClear(GL_COLOR_BUFFER_BIT);

        /**
         * 双缓冲(Double Buffer)
         * 应用程序使用单缓冲绘图时可能会存在图像闪烁的问题。
         * 这是因为生成的图像不是一下子被绘制出来的，而是按照从左到右，由上而下逐像素地绘制而成的。
         * 最终图像不是在瞬间显示给用户，而是通过一步一步生成的，这会导致渲染的结果很不真实。
         * 为了规避这些问题，我们应用双缓冲渲染窗口应用程序。
         * 前缓冲保存着最终输出的图像，它会在屏幕上显示；
         * 而所有的的渲染指令都会在后缓冲上绘制。
         * 当所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓冲，这样图像就立即呈显出来，之前提到的不真实感就消除了。
         */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /**
     * 释放/删除之前的分配的所有资源
     */
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}