#pragma once

//1	单例类（全局唯一实例）
//2	成员变量 + 成员函数
//	2.1 成员函数-init（初始化）
//	2.3 成员函数-destroy（结尾执行）
//3	响应回调函数(Resize)
//	3.1 声明一个函数指针ResizeCallback
//	3.2 声明一个ResizeCallback类型的成员变量
//	3.3 声明一个SetResizeCallback的函数 ，设置窗体变化响应回调函数
//	3.4 声明一个static的静态函数，用于响应glfw窗体变化
//	3.5 将静态函数设置到glfw的监听Resize监听当中
//  3.6 *学会使用glfw的UserPointer
//4	响应键盘消息函数(KeyBoard)
//	3.1 声明一个static的静态函数，用于响应glfw的键盘事件
//	3.2 将静态函数设置到glfw的监听KeyCallback监听当中
//	3.3 声明一个函数指针KeyBoardCallback
//	3.4 声明一个KeyBoardCallback类型的成员变量
//	3.5 声明一个SetKeyBoardCallback的函数 ，设置键盘响应回调函数
//	3.6 * 学会使用glfw的UserPointer

#include <iostream>

#define app Application::getInstance() //宏定义替换app等价于Application::getInstance()

class GLFWwindow;

//创建窗口尺寸的ResizeCallback的函数指针，该指针有两个int参数，返回值为void*
using ResizeCallback = void(*)(int width, int height);
//创建窗口按键的ResizeCallback的函数指针，该指针有三个int参数，返回值为void*
using KeyBoardCallback = void(*)(int key, int action, int mods);

class Application {
public:
	~Application();
	//用于访问实例的静态函数
	static Application* getInstance();
	//获取宽高的接口
	uint32_t getWidth()const { return mWidth; }
	uint32_t getHeight()const { return mHeight; }
	//初始化(只需要做三件事且只执行一次)
	//1.设置初始信息
	//2.生成窗体
	//3.载入opengl函数
	bool init(const int& width = 800, const int& height = 600);
	//帧刷新(每帧需要做的事情)
	//1.更新双缓存
	//2.接收并分发窗体消息
	bool update();
	//结尾资源清理-执行glfwTerminate,推出gl环境
	void destroy();
	//设置窗体变化响应回调函数,将外部与函数指针要求一致的函数,设置到Application对象中
	void setResizeCallback(ResizeCallback callback) { mResizeCallback = callback; }
	//设置键盘响应回调函数
	void setKeyBoardCallback(KeyBoardCallback callback) { mKeyBoardCallback = callback; }
private:
	//下方两个函数为什么需要使用静态函数？？？？？
	//1.C/C++函数调用约定差异
	//  1.1 GLFW作为C库，其回调机制需要符合 C函数调用约定
	//  1.2 非静态成员函数隐含this指针参数，与C函数指针类型不兼容
	//  1.3 静态成员函数没有this指针，其函数签名与C函数兼容
	//2.对象实例访问的实现
	//  //初始化时存储对象指针
	//    glfwSetWindowUserPointer(window, this);
	//  //回调中取回对象指针
	//    Application* self = static_cast<Application*>(glfwGetWindowUserPointer(window));
	static void frameBufferSizeCallback(GLFWwindow* window,int width, int height);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	Application();//防止外界New出Application实例
	static Application* mInstance; //全局唯一的静态变量实例
	uint32_t mWidth{ 0 };          //宽
	uint32_t mHeight{ 0 };         //高  
	GLFWwindow* mWindow{ nullptr };//存储当前窗体对象
	ResizeCallback mResizeCallback{ nullptr };
	KeyBoardCallback mKeyBoardCallback{ nullptr };
};