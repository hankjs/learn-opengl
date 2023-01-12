#include <iostream>
#include "SandboxWindow.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Utils.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"

#include "Camera.h"

#pragma region Model Data
/**
 * 矩形形的顶点坐标
 */
float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

unsigned int indices[] = {
    // 注意索引从0开始!
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形
    0, 1, 2, // 第一个三角形
    0, 2, 3  // 第二个三角形
};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)};
#pragma endregion

#pragma region Camera Declare
// 初始化Camera
// Camera camera(glm::vec3(0, 0, 3.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1.0f, 0));
// 欧拉角Camera
Camera camera(glm::vec3(0, 0, 3.0f), glm::radians(30.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));
#pragma endregion // Camera Declare

#pragma region Input Declare
float lastX = 400.0f, lastY = 300.0f;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    if(firstMouse)
    {
        lastX = (float)xPos;
        lastY = (float)yPos;
        firstMouse = false;
    }
    float deltaX, deltaY;
    deltaX = (float)xPos - lastX;
    deltaY = (float)yPos - lastY;
    lastX = (float)xPos;
    lastY = (float)yPos;

    camera.ProcessMouseMovement(deltaX, -deltaY);
}
#pragma endregion // Input Declare

unsigned int loadImageToGPU(const char* filename, GLint internalFormat, GLenum format, int textureSlot) {
        /**
     * Texture 绑定在VBO之后，
     * OpenGL是个状态机,其实只需要在init之后上下文可以使用了就可以绑定。
     * 不过考虑到渲染流程，设定资源先后加载顺序可以更直觉。
    */
    unsigned int texBuffer;
    glGenTextures(1, &texBuffer);
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, texBuffer);
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
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texBuffer;
}

int main()
{
#pragma region Open a Window
//         glfwInit();
//         /** 设定OpenGL 版本: 3.3 */
//         glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 指定OpenGL主版本
//         glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 指定OpenGL次要版本
//         glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//         /** MacOS 需要下面这行 */
// #ifdef __APPLE__
//         glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

//         /** 创建一个窗口对象 */
//         GLFWwindow *_window = glfwCreateWindow(800, 600, "Hank", NULL, NULL);

    SandboxWindow::SandboxWindow sandboxWindow(800, 600, "LearnOpenGL");

    sandboxWindow.onMouse(mouse_callback);
    sandboxWindow.initGLAD();
    sandboxWindow.setViewport();
    sandboxWindow.autoResize();
#pragma endregion // Open a Window

#pragma region Init Shader Program
    {
        /** 打印OpenGL版本 */
        unsigned char *glVersion;
        glVersion = (unsigned char *)glGetString(GL_VERSION);
        std::cout << "Status: Using GL " << glVersion << std::endl;
    }

    Shader *shader = new Shader("shader/vertexShader.vert", "shader/fragmentShader.frag");
#pragma endregion // init Shader Program

#pragma region Init and Load Models to VAO, VBO
    VertexArray vertexArray;
    VertexBuffer vertexBuffer(vertices, sizeof(vertices));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    vertexArray.AddBuffer(vertexBuffer, layout);
#pragma endregion // Init and Load Models to VAO, VBO

#pragma region Init Textures
    /**
     * Texture 绑定在VBO之后，
     * OpenGL是个状态机,其实只需要在init之后上下文可以使用了就可以绑定。
     * 不过考虑到渲染流程，设定资源先后加载顺序可以更直觉。
    */
    unsigned int texBufferA;
    texBufferA = loadImageToGPU("assets/container.jpg", GL_RGB, GL_RGB, 0);

    unsigned int texBufferB;
    texBufferB = loadImageToGPU("assets/awesomeface.png", GL_RGBA, GL_RGBA, 1);
#pragma endregion // Init Textures

#pragma region Prepare MVP matrices
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
#pragma endregion // Prepare MVP matrices

    // 只绘制线框
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /**
     * 启用深度测试
     */
    GLCall(glEnable(GL_DEPTH_TEST));

    while (!sandboxWindow.isClose())
    {
        // Process Input
        sandboxWindow.processInput();

        // Clear Screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        viewMatrix = camera.GetViewMatrix();
        for(unsigned int i = 0; i < 10; i++)
        {
            // Set Model matrix
            modelMatrix = glm::translate(glm::mat4(1.0f), cubePositions[i]);

            // Set View and Projection Matrices here if you want.

            // Set Material -> Shader Program
            shader->use();
            // Set Material -> Textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texBufferA);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texBufferB);
            // Set Material -> Uniforms
            shader->setInt("ourTexture", 0);
            shader->setInt("ourFace", 1);
            shader->setMat4("modelMatrix", modelMatrix);
            shader->setMat4("viewMatrix", viewMatrix);
            shader->setMat4("projectionMatrix", projectionMatrix);

            // 在此之前不要忘记首先 use 对应的着色器程序（来设定uniform）
            shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
            shader->setVec3("ambientColor", 1.0f, 1.0f, 1.0f);

            // Set Model
            vertexArray.Bind();

            // Drawcall
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        sandboxWindow.refresh();
    }

    return 0;
}
