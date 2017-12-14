#include"Light.h"

DirectionalLight::DirectionalLight() {
	this->power = 1.0f;
	this->color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	this->direction = Vector3f(0.0f, 0.0f, 1.0f);
}

void DirectionalLight::SetLight(ShaderClass* shader) {
	shader->SetDirectionalLight(this->power, this->color, this->direction);
}

void DirectionalLight::SetPower(float power) {
	this->power = power;
}
void DirectionalLight::SetColor(float r, float g, float b, float a) {
	this->color = Vector4f(r, g, b, a);
}
void DirectionalLight::SetDirection(float x, float y, float z) {
	this->direction = Vector3f(x, y, z);
}

float DirectionalLight::GetPower() {
	return this->power;
}
const Vector4f& DirectionalLight::GetColor() {
	return this->color;
}
const Vector3f& DirectionalLight::GetDirection() {
	return this->direction;
}