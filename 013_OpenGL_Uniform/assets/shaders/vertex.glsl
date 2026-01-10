#version 460 core  // 声明使用的 GLSL 版本为 460
// 顶点着色器：处理每个顶点的坐标和颜色，输出顶点的变换位置和颜色信息
// 输入变量
layout (location = 0) in vec3 aPos;   // 定义输入变量 aPos，用于传入顶点坐标 (位置)，位置索引为 0
layout (location = 1) in vec3 aColor; // 定义输入变量 aColor，用于传入顶点颜色，位置索引为 1

//方式1，通过修改Vertex颜色，传入fragment
//uniform float time;

// 输出变量
out vec3 color;  // 定义输出变量 color，将传递到片段着色器中

void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); // 将输入的顶点位置 aPos 转换为四维向量并赋值给 gl_Position
   color = aColor;  // 将输入的颜色 aColor 传递给输出变量 color(原版)

   //方式1测试
   //color=aColor*(sin(time)+1.0)/2.0;

   //方式2通过flagment实现渐变三角形

}
