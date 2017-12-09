#pragma once
#include<Eigen\Core>
#include"ShaderClass.h"
#include"TextureList.h"

using namespace Eigen;
//ƒ‚ƒfƒ‹Šî’êƒNƒ‰ƒX
class Model {
public:
	Model();
	virtual ~Model();
	virtual void Draw() = 0;
	virtual void InstanceDraw(int numInstance) = 0;
	void SetMatrix(Matrix4f& world, Matrix4f& view, Matrix4f& projection);
	void SetShader(ShaderClass* shader);
protected:
	TextureList* textureList;
	ShaderClass* shader;
	Matrix4f world;
	Matrix4f view;
	Matrix4f projection;
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};