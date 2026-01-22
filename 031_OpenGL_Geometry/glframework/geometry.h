#pragma once

#include "core.h"
//面向学习：我们需要提供基础几何物体的geometry对象创建接口(立方体+经纬球)
class Geometry {
public:
	Geometry();
	~Geometry();
	static Geometry* createBox(float size);     //创建立方体
	static Geometry* createSphere(float radius);//创建经纬球
	GLuint getVao()const { return mVao; }       //外界绑定VAO(常函数：不可以修改参数的值)                  
	uint32_t getIndicesCount()const { return mIndicesCount; }//需要知道绘制多少index
private:
	GLuint mVao{ 0 };
	GLuint mPosVbo{ 0 };
	GLuint mUvVbo{ 0 };
	GLuint mEbo{ 0 };
	uint32_t mIndicesCount{ 0 };//记录索引，便于绘制
};