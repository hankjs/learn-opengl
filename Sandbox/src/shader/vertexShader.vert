#version 330 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

// out vec4 vertexColor; // 为片段着色器指定一个颜色输出
out vec2 TexCoord; // 原封不动的输出到fragment

// uniform mat4 transform;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
  // gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
  // vertexColor = vec4(aColor.r, aColor.g, aColor.b, 1.0); // 把输出变量设置为暗红色
  TexCoord = aTexCoord;
}