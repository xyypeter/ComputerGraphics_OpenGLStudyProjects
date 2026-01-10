#include <iostream>
#include <glad/glad.h>//GLAD的头文件引用需要放在GLFW上
#include <GLFW/glfw3.h>
#include "wrapper/checkError.h"
#include "application/application.h"
using namespace std;

void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	std::cout << key << std::endl;
}

int main() {
	//窗口初始化
	if (!app->init(800, 600)) {
		return -1;
	}
	//调用窗口响应和键盘响应
	app->setResizeCallback(OnResize);
	app->setKeyBoardCallback(OnKey);
	//设置opengl视口以及清理颜色
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	while (app->update()) {
		//执行opengl画布清理操作
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
		//渲染操作
		//TODO-----------
	}
	//推出前进行相关清理
	app->destroy();
	return 0;

}