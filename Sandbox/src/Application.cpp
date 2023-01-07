#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

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

    Shader *shader = new Shader("shader/vertexShader.glsl", "shader/fragmentShader.glsl");

    /**
     * 矩形形的顶点坐标
    */
    float vertices[] = {
        //  --- 位置 ---        --- 颜色 ---       - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f // 左上
    };

    unsigned int indices[] = {
        // 注意索引从0开始! 
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形
        0, 1, 2, // 第一个三角形
        0, 2, 3 // 第二个三角形
    };

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    /**
     * 顶点缓冲对象(Vertex Buffer Objects, VBO
    */
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    /**
     * OpenGL有很多缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER。
     * OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型。
     * 我们可以使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上：
    */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /**
     * 从这一刻起，我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。
     * 然后我们可以调用glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存中：
    */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    /**
     * Texture 绑定在VBO之后，
     * OpenGL是个状态机,其实只需要在init之后上下文可以使用了就可以绑定。
     * 不过考虑到渲染流程，设定资源先后加载顺序可以更直觉。
    */
    unsigned int texBufferA;
    glGenTextures(1, &texBufferA);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texBufferA);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width;
    int height;
    int nrChannels;
    /**
     * stbi Y轴反转
    */
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    unsigned int texBufferB;
    glGenTextures(1, &texBufferB);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texBufferB);
    data = stbi_load("assets/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    /** 位移 */
    // trans = glm::translate(trans, glm::vec3(-0.0001f, 0, 0));
    // 旋转
    // trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    /** 缩放 */
    // trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    glm::mat4 modelMatrix;
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0, 0));
    glm::mat4 viewMatrix;
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 0, -3.0f));
    glm::mat4 projectionMatrix;
    projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // 只绘制线框
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /**
     * glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序了。
     * glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
     * glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
     */
    while (!glfwWindowShouldClose(window))
    {
        // glm::mat4 trans;
        // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
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

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texBufferA);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texBufferB);

        shader->use();
        glUniform1i(glGetUniformLocation(shader->ID, "ourTexture"), 0);
        glUniform1i(glGetUniformLocation(shader->ID, "ourFace"), 1);
        // glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
        glUniformMatrix4fv(glGetUniformLocation(shader->ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shader->ID, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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