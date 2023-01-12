#include "VertexBuffer.h"
#include "Utils.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
    /**
     * 顶点缓冲对象(Vertex Buffer Objects, VBO
    */
    GLCall(glGenBuffers(1, &m_RendererID));                            /* 生成缓冲区 */
    /**
     * OpenGL有很多缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER。
     * OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型。
     * 我们可以使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上：
    */
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));               /* 绑定缓冲区 */
    /**
     * 从这一刻起，我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。
     * 然后我们可以调用glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存中：
    */
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); /* 设置缓冲区数据 */
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); /* 绑定缓冲区 */
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); /* 绑定缓冲区 */
}
