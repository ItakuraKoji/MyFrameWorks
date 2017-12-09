#include"GameObject.h"

GameObject::GameObject() {
	this->position = Vector3f(0, 0, 0);
	this->rotation = Vector3f(0, 0, 0);
	this->scale    = Vector3f(0, 0, 0);

	this->drawModel = 0;
}
GameObject::~GameObject() {
	if (this->drawModel) {
		delete this->drawModel;
	}
}

void GameObject::SetDrawModel(MeshModel* model) {
	this->drawModel = model;
}
void GameObject::SetPosition(float x, float y, float z) {
	this->position << x, y, z;
}
void GameObject::SetRotation(float x, float y, float z) {
	this->rotation << x, y, z;
}
void GameObject::SetScale(float x, float y, float z) {
	this->scale << x, y, z;
}

Vector3f GameObject::GetPosition() {
	return this->position;
}
Vector3f GameObject::GetRotation() {
	return this->rotation;
}
Vector3f GameObject::GetScale() {
	return this->scale;
}

////////
//protected
////
void GameObject::SetMatrix(DrawParameters& param) {
	//ƒ[ƒ‹ƒhs—ñ
	Translation<float, 3> trans = Translation<float, 3>(this->position);
	//Quaternionf rot = Quaternionf(this->rotation);


	ShaderClass* shader = param.shaderList->GetShader(this->shaderName);


}