#pragma once
#include<Eigen\Core>
#include"ShaderClass.h"

using namespace Eigen;
//モデル基底クラス
class Model {
public:
	Model();
	virtual ~Model();
	virtual void Draw() = 0;
	void SetMatrix(Matrix4f& world, Matrix4f& view, Matrix4f& projection);
	void SetShader(ShaderClass* shader);
protected:
	ShaderClass *shader;
	Matrix4f world;
	Matrix4f view;
	Matrix4f projection;
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};