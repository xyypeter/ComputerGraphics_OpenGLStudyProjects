#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Application静态变量的初始化，设置在此的好处如下:
//1.避免重复定义（ODR规则）
//  如果初始化写在.h文件中，当该头文件被多个.cpp文件包含时，会导致静态变量被多次定义，导致链接报"multiple definition"错误
//2.针对单例模式的特殊控制
//  确保在getInstance()首次调用时才真正初始化
//
Application* Application::mInstance = nullptr;

Application::Application() {}

Application::~Application() {}

Application* Application::getInstance() {
	if (mInstance == nullptr) {
		mInstance = new Application();
	}
	return mInstance;
}

bool Application::init(const int& width, const int& height){
	mWidth = width;
	mHeight = height;
	//1 初始化GLFW基本环境
	glfwInit();
	//1.1 设置OpenGL主版本号、次版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//1.2 设置OpenGL启用核心模式（非立即渲染模式）
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//2 创建窗体对象
	mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGLStudy", NULL, NULL);
	if (mWindow == NULL) {
		return false;
	}
	//2.1设置当前窗体对象为OpenGL的绘制舞台
	glfwMakeContextCurrent(mWindow);
	//加载openGL相关函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	//将当前Application实例的指针存储在GLFW窗口对象中
    //设计原理如下
    //1.上下文传递：解决C回调无法直接访问C++对象的问题
    //2.数据关联：建立 GLFWwindow* 与 Application* 的映射关系
    //3.类型安全转换：后续通过 glfwGetWindowUserPointer 取回指针时需要进行强制类型转换
	glfwSetWindowUserPointer(mWindow, this);

	//将GLFW窗口的帧缓冲大小变化事件绑定到静态成员函数 frameBufferSizeCallback
	//设计原理如下
	//1.C库兼容性：GLFW是C语言库，只能接受普通函数指针或静态成员函数作为回调
	//2.事件路由：当窗口尺寸变化时，GLFW通过此函数指针调用 frameBufferSizeCallback
	//3.间接访问实例：通过后续的glfwSetWindowUserPointer实现C++对象访问
	glfwSetFramebufferSizeCallback(mWindow, frameBufferSizeCallback);

	//键盘响应
	glfwSetKeyCallback(mWindow, KeyCallback);
	return true;
}

bool Application::update(){
	//检查GLFW窗口是否被关闭
	if (glfwWindowShouldClose(mWindow)) {
		return false;
	}
	//接收并分发窗体消息,检查消息队列是否有需要处理的鼠标、键盘等消息,如果有的话就将消息批量处理，清空队列
	glfwPollEvents();
	//切换双缓存
	glfwSwapBuffers(mWindow);
	return true;
}

void Application::destroy(){
	//退出程序前做相关清理
	glfwTerminate();
}

void Application::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	//glfwGetWindowUserPointer：在回调中取出指针，实现C风格回调访问C++对象
	if (self->mResizeCallback != nullptr) {
		self->mResizeCallback(width, height);
	}
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	//glfwGetWindowUserPointer：在回调中取出指针，实现C风格回调访问C++对象
	if (self->mKeyBoardCallback != nullptr) {
		self->mKeyBoardCallback(key, action, mods);
	}
}
