#include"Light.h"
AmbientLight::AmbientLight() {
	this->power = 1.0f;
	this->color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
}
void AmbientLight::SetLight(ShaderClass* shader) {
	shader->SetAmbientLight(this->power, this->color);
}
void AmbientLight::SetPower(float power) {
	this->power = power;
}
void AmbientLight::SetColor(float r, float g, float b, float a) {
	this->color = Vector4f(r, g, b, a);
}
float AmbientLight::GetPower() {
	return this->power;
}
const Vector4f& AmbientLight::GetColor() {
	return this->color;
}