#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec4 vertexColor; // 为片段着色器指定一个颜色输出

void main()
{
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  vertexColor = vec4(aColor.r, aColor.g, aColor.b, 1.0); // 把输出变量设置为暗红色
}