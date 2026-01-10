#include <iostream>
#include <glad/glad.h>//坑：头文件引用，glad需要放在GLFW上
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

void prepareSingleBuffer() {
	//此函数描述的VAO结构如下
	//    
	//     位置描述信息->posVbo 
	//    /
	//VAO
	//    \颜色描述信息->colorVbo
	//

	//1 准备VBO顶点位置数据与颜色数据
	float positions[] = {
		   -0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
	};
	float colors[] = {
	   1.0f, 0.0f, 0.0f,
	   0.0f, 1.0f, 0.0f,
	   0.0f, 0.0f, 1.0f
	};
	//2 为位置&颜色数据各自生成一个vbo,绑定对应内容
	GLuint posVbo = 0, colorVbo = 0;
	GL_CALL(glGenBuffers(1, &posVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));
	GL_CALL(glGenBuffers(1, &colorVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW));
	//3 生成VAO并且绑定
	GLuint vao = 0;
	//glGenVertexArrays参数解析如下:
	//参数1 n:表示创建多少个VAO
	//参数2 arrays:表示创建出来的VAO编号们，都放在arrays指向的数组中
	GL_CALL(glGenVertexArrays(1, &vao));   
	//绑定要绑定的VAO编号
	GL_CALL(glBindVertexArray(vao));

	//4 分别将位置/颜色属性的描述信息加入vao中
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));//此行的目的是，只有绑定了posVbo,以下属性才会生效
	GL_CALL(glEnableVertexAttribArray(0));         //位置属性存放在vao属性的0号位置，需要先激活
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	//void glVertexAttribPointer(GLuint index,GLuint size,GLenum type,GLboolean normalized,GLsizei stride,const void* pointer);
	// 参数index:要描述第几个信息
	// 参数size: 属性包含几个数字
	// 参数type: 属性的每个数字是什么数据类型
	// 参数normailzed:是否归一化，暂时不考虑     GL_FALSE
	// 参数stride: 每个顶点数据的步长 
	// 参数pointer:属性在顶点数据内的偏移量      SingleBuffer不存在内部偏移
	// 

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVbo));
	GL_CALL(glEnableVertexAttribArray(1));         
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	//5 将当前VAO解绑(建议操作)
	GL_CALL(glBindVertexArray(0));
}

void prepareInterleavedBuffer() {
	//1 准备好Interleaved数据（位置+颜色）
	float vertices[] = {
		   -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};
	//2 创建唯一的vbo
	GLuint vbo = 0;
	GL_CALL(glGenBuffers(1, &vbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW));
	//3 创建并绑定vao
	GLuint vao = 0;
	GL_CALL(glGenVertexArrays(1, &vao));
	GL_CALL(glBindVertexArray(vao));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));//只有绑定了posVbo,以下两个描述属性才会生效*********核心点不能丢

	//4 为vao加入位置和颜色的描述信息
	//4.1 位置描述信息
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));

	//4.2 颜色描述信息
	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
	//注意上述最后一项参数，(void*)(3 * sizeof(float))) 因为颜色属性相对顶点位置，偏移量为3
	//之所以传入(void*)是由于历史的原因

	//5 将当前VAO解绑(建议操作)
	GL_CALL(glBindVertexArray(0));
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

	//VAO测试
	prepareSingleBuffer();
	prepareInterleavedBuffer();

	while (app->update()) {
		//执行opengl画布清理操作
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
		//渲染操作
		//TODO-----------
	}
	
	app->destroy();
	return 0;

}