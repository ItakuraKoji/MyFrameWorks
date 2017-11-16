#pragma once
#include<Eigen\Core>
#include"ShaderClass.h"

using namespace Eigen;
//ライトのデータを保持(ディレクショナル)
class DirectionalLight {
public:
	DirectionalLight();
	~DirectionalLight();
	void SetDirection(float x, float y, float z);
	void SetColor(float r, float g, float b, float a);
	void SetShader(ShaderClass* shader);

private:
	Vector3f direction;
	Vector4f color;
};
