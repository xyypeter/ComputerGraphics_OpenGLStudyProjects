#version 460 core  // 声明使用的 GLSL 版本为 460

// 输出变量
// 片元着色器：接收顶点着色器传递的颜色信息，生成最终的像素颜色输出
out vec4 FragColor; // 定义输出变量 FragColor，用于设置最终像素的颜色

// 输入变量
in vec3 color;   // 接收来自顶点着色器的颜色输出
in vec3 position;// 接收来自顶点着色器的位置输出

void main()
{
   //FragColor = vec4(color, 1.0f); // 将输入的颜色 color 转换为四维向量，并将不透明度设置为 1.0
   FragColor = vec4(position, 1.0f);
}
