#version 460 core  // 声明使用的 GLSL 版本为 460
// 顶点着色器：处理每个顶点的坐标和颜色，输出顶点的变换位置和颜色信息
// 输入变量
layout (location = 0) in vec3 aPos;   // 定义输入变量 aPos，用于传入顶点坐标 (位置)，位置索引为 0
layout (location = 1) in vec3 aColor; // 定义输入变量 aColor，用于传入顶点颜色，位置索引为 1

uniform float time;
uniform float speed;

// 输出变量
out vec3 color;  // 定义输出变量 color，将传递到片段着色器中

void main()
{
   float dx=0.3;
   //float offserX=sin(time)*dx;
   float offserX=sin(time*speed)*dx;
   gl_Position = vec4(aPos.x+offserX, aPos.y, aPos.z, 1.0); // 将输入的顶点位置 aPos 转换为四维向量并赋值给 gl_Position
   color = aColor;  // 将输入的颜色 aColor 传递给输出变量 color
}
