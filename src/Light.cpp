#include"Light.h"

DirectionalLight::DirectionalLight() {

}
DirectionalLight::~DirectionalLight() {

}

void DirectionalLight::SetDirection(float x, float y, float z) {
	this->direction = Vector3f(x, y, z);
}
void DirectionalLight::SetColor(float r, float g, float b, float a) {
	this->color = Vector4f(r, g, b, a);
}
void DirectionalLight::SetShader(ShaderClass* shader) {
	shader->SetAmbientLight(0.5f, Vector4f(1.0f, 1.0f, 0.0f, 1.0f));
	shader->SetDirectionalLight(0.3f, this->color, this->direction);
}