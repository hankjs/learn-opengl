#version 330 core
out vec4 FragColor;

// in vec4 vertexColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）
in vec2 TexCoord; // uv

uniform sampler2D ourTexture;
uniform sampler2D ourFace;

uniform vec3 objectColor;
uniform vec3 ambientColor;

void main()
{
    vec4 face = texture(ourFace, TexCoord);
    FragColor = vec4(ambientColor * objectColor, 1.0) * mix(texture(ourTexture, TexCoord), face, face.a * 0.2);
}