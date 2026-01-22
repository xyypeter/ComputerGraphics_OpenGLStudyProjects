#include "geometry.h"
#include <vector>

Geometry::Geometry() {

}

Geometry::~Geometry() {
	if (mVao != 0) {
		glDeleteVertexArrays(1, &mVao);
	}
	if (mPosVbo != 0) {
		glDeleteBuffers(1, &mPosVbo);
	}
	if (mUvVbo != 0) {
		glDeleteBuffers(1, &mUvVbo);
	}
	if (mEbo != 0) {
		glDeleteBuffers(1, &mEbo);
	}
}

Geometry* Geometry::createBox(float size) {
	//1 geometry创建，以及参数的准备
	Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 36;//一个面两个三角形，6个面，6个索引，所以为36
	float halfSize = size / 2.0f;//单边长度为size,取一半
	float positions[] = {//位置属性
		// Front face
		-halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize,
		// Back face
		-halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize,
		// Top face
		-halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize,
		// Bottom face
		-halfSize, -halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize,
		// Right face
		halfSize, -halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize,
		// Left face
		-halfSize, -halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize
	};
	// 对于前面 (Front face)
	//0.0, 0.0(左下角) 1.0, 0.0(右下角)  1.0, 1.0(右上角) 0.0, 1.0  (左上角)
	float uvs[] = {//UV属性，用于表示当前像素/顶点对应图片上百分比位置的横纵百分比
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,// Front face
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,// Back face
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,// Top face
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,// Bottom face
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,// Right face
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,// Left face
	};
	unsigned int indices[] = {
		0, 1, 2, 2, 3, 0,       // Front face
		4, 5, 6, 6, 7, 4,       // Back face
		8, 9, 10, 10, 11, 8,    // Top face
		12, 13, 14, 14, 15, 12, // Bottom face
		16, 17, 18, 18, 19, 16, // Right face
		20, 21, 22, 22, 23, 20  // Left face
	};

	//2 VBO创建,存储Mesh顶点属性数据，位置，UV，颜色(VBO--->仓库)
	GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	//3 EBO创建，存储顶点绘制顺序的索引号的GPU显存区域(复用工具--->告诉OpenGL如何链接顶点)
	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//4 VAO创建，存储对Mesh所有属性的描述(指挥官--->不存储实际数据,而是记录绑定了那些VBO和EBO)
	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);    

	//5 将posVbo绑定至顶点数据缓冲区，之后再次传输顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);//设置着色器的位置索引为0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	
	//6 将纹理坐标 VBO（uvVbo）绑定到 GL_ARRAY_BUFFER 目标上
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);//设置着色器的位置索引为1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	//7 加入ebo到当前的vao,指示顶点数组中的那些顶点要组合成三角形
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBindVertexArray(0);//解出绑定
	return geometry;
}

Geometry* Geometry::createSphere(float radius) {
	Geometry* geometry = new Geometry();

	//目标：1 位置 2 uv 3 索引
	//1 主要变量声明
	std::vector<GLfloat> positions{};
	std::vector<GLfloat> uvs{};
	std::vector<GLuint> indices{};

	//声明纬线与经线的数量
	int numLatLines = 60;//纬线
	int numLongLines = 60;//经线

	//2 通过两层循环
	for (int i = 0; i <= numLatLines; i++) {      //纬线
		for (int j = 0; j <= numLongLines; j++) { //经线
			//角度计算
			float phi = i * glm::pi<float>() / numLatLines;       //经线角计算(Π)
			float theta = j * 2 * glm::pi<float>() / numLongLines;//纬线角计算(2Π)
			//顶点位置坐标计算
			float y = radius * cos(phi);
			float x = radius * sin(phi) * cos(theta);
			float z = radius * sin(phi) * sin(theta);
			positions.push_back(x);
			positions.push_back(y);
			positions.push_back(z);
			//UV坐标
			float u = 1.0 - (float)j / (float)numLongLines;
			float v = 1.0 - (float)i / (float)numLatLines;
			//float u = (float)j / (float)numLongLines;
			//float v = (float)i / (float)numLatLines;
			uvs.push_back(u);
			uvs.push_back(v);
		}
	}

	//3 通过两层循环（这里没有=号）->顶点索引
	for (int i = 0; i < numLatLines; i++) {
		for (int j = 0; j < numLongLines; j++) {
			int p1 = i * (numLongLines + 1) + j;
			//i：当前纬度线编号（范围 [0, numLatLines]）
			//j：当前经度线编号（范围 [0, numLongLines]）
			//numLongLines + 1：每行（纬度线）的顶点数（因为经线是闭合的，首尾顶点重合，所以需要 +1）
			int p2 = p1 + numLongLines + 1;
			int p3 = p1 + 1;
			int p4 = p2 + 1;

			indices.push_back(p1);
			indices.push_back(p2);
			indices.push_back(p3);

			indices.push_back(p3);
			indices.push_back(p2);
			indices.push_back(p4);
		}
	}

	//4 生成vbo与vao
	GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBindVertexArray(0);
	geometry->mIndicesCount = indices.size();
	return geometry;
}
