#pragma once
#include<Eigen\Core>
#include"ShaderClass.h"

using namespace Eigen;

class AmbientLight{
public:
	AmbientLight();
	void SetLight(ShaderClass* shader);

	void SetPower(float power);
	void SetColor(float r, float g, float b, float a);

	float GetPower();
	const Vector4f& GetColor();

protected:
	float    power;
	Vector4f color;
};

class DirectionalLight{
public:
	DirectionalLight();
	void SetLight(ShaderClass* shader);

	void SetPower(float power);
	void SetColor(float r, float g, float b, float a);

	float GetPower();
	const Vector4f& GetColor();

	void  SetDirection(float x, float y, float z);
	const Vector3f& GetDirection();

protected:
	float    power;
	Vector4f color;
	Vector3f direction;
};

