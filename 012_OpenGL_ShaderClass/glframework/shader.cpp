#include "shader.h"
#include "shader.h"
#include "../wrapper/checkError.h"//..表示退出当前文件夹，切换其他文件夹(wrapper),然后进入选择的文件夹
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//shader构造函数
// 参数一：vertexshader读取路径
// 参数二: fragmentshader读取路径
Shader::Shader(const char* vertexPath, const char* fragmentPath){
	//声明字符串：装入shader代码字符串的两个string
	std::string vertexCode;
	std::string fragmentCode;
	//声明文件流：用于读取vs跟fs文件的inFileStream
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	//设置异常处理：保证ifstream遇到问题的时候可以抛出异常
	//exceptions() 方法：
	//	这是std::ios_base类的成员函数(所有流类的基类),用于设置流对象在哪些错误状态位被设置时抛出异常
	//参数说明（通过位或操作 | 组合）：
	//	std::ifstream::failbit：表示逻辑错误（如类型转换失败）
	//	std::ifstream::badbit：表示严重错误（如文件无法读取、流损坏）
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	//当这些错误状态位被设置时，流对象会抛出std::ios_base::failure异常,需要配合try - catch块使用
	try {
		//1 打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		//2 读取文件：将文件输入流当中的字符串输入到stringStream里面
		//vShaderFile.rdbuf() 获取文件流的底层缓冲区
		//<<操作符将缓冲区内容全部提取出来
		//内容被输出到vShaderStream字符串流中
		//最终vShaderStream将包含文件的完整内容
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//3 关闭文件
		vShaderFile.close();
		fShaderFile.close();

		//4 转换为字符串：将字符串从stringStream当中读取出来，转化到code String当中
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	//捕捉文件异常
	catch (std::ifstream::failure& e) {
		std::cout << "ERROR: Shader File Error: " << e.what() << std::endl;
	}
	//为什么一定需要上述错误状态的处理？？？
	//OpenGL着色器加载：在图形编程中，着色器文件加载失败必须被明确处理

	//声明ShaderSource
	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();
	//1 创建Shader程序（vs、fs）
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	//2 为shader程序输入shader代码
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
	int success = 0;
	char infoLog[1024];
	//3 执行shader代码编译,
	glCompileShader(vertex);
	//检查vertex编译结果和fragment编译结果
	checkShaderErrors(vertex, "COMPILE");
	glCompileShader(fragment);
	checkShaderErrors(fragment, "COMPILE");
	//4 创建一个Program壳子(完整的着色器套装容器)
	mProgram = glCreateProgram();
	//5 将vs与fs编译好的结果放到program这个壳子里
	glAttachShader(mProgram, vertex);
	glAttachShader(mProgram, fragment);
	//6 执行program的链接操作，形成最终可执行shader程序
	glLinkProgram(mProgram);
	//检查链接错误
	checkShaderErrors(mProgram, "LINK");
	//清理
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader(){}

void Shader::begin(){
	GL_CALL(glUseProgram(mProgram));//开始使用当前的Shader
}

void Shader::end(){
	GL_CALL(glUseProgram(0));       //结束使用当前的Shader
}

void Shader::checkShaderErrors(GLuint target, std::string type) {
	int success = 0; // 用于存储编译或链接状态的变量
	char infoLog[1024]; // 用于存储错误信息的字符数组
	// 检查类型是否为 "COMPILE"编译
	if (type == "COMPILE") {
		glGetShaderiv(target, GL_COMPILE_STATUS, &success);  // 获取着色器的编译状态
		if (!success) { // 如果编译未成功
			glGetShaderInfoLog(target, 1024, NULL, infoLog); // 获取编译错误信息
			std::cout << "Error: SHADER COMPILE ERROR" << "\n" << infoLog << std::endl; // 输出错误信息
		}
	}
	// 检查类型是否为 "LINK" 链接
	else if (type == "LINK") {
		glGetProgramiv(target, GL_LINK_STATUS, &success); // 获取程序的链接状态
		if (!success) { // 如果链接未成功
			glGetProgramInfoLog(target, 1024, NULL, infoLog); // 获取链接错误信息
			std::cout << "Error: SHADER LINK ERROR " << "\n" << infoLog << std::endl; // 输出错误信息
		}
	}else {
		std::cout << "Error: Check shader errors Type is wrong" << std::endl; // 如果类型不匹配，输出错误信息 输出错误类型提示
	}
}

