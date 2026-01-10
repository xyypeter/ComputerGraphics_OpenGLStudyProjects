#pragma once
#include "core.h"
#include <string>

class Shader {
public:
	Shader(const char* vertexPath,const char* fragmentPath);
	~Shader();
	void begin();//开始使用当前Shader
	void end();  //结束使用当前Shader
	void setFloat(const std::string& name, float value);
	void setVector3(const std::string& name, float x, float y, float z);
	void setVector3(const std::string& name, const float* values);
	void setInt(const std::string& name, int value);
private:
	//shader program
    //type:COMPILE LINK
	//target:表示 OpenGL 对象的 ID，通常是一个着色器对象或程序对象的标识符,指定需要检查错误的具体着色器对象
	//type:表示着色器的类型，通常为 "VERTEX"、"FRAGMENT" 等字符串,在输出错误信息时，帮助识别是哪个类型的着色器发生了错误
	void checkShaderErrors(GLuint target, std::string type);
private:
	GLuint mProgram{ 0 };
};

