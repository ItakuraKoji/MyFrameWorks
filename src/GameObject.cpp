#include"GameObject.h"

GameObject::GameObject() {
	this->position << 0.0f, 0.0f, 0.0f;
	this->rotation << 0.0f, 0.0f, 0.0f;
	this->scale << 1.0f, 1.0f, 1.0f;
	this->drawModel = 0;
}
GameObject::~GameObject() {
	if (this->drawModel) {
		delete this->drawModel;
	}
}

void GameObject::SetDrawModel(MeshObject* model) {
	if (this->drawModel) {
		delete this->drawModel;
	}
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

MeshObject* GameObject::GetModel() {
	return this->drawModel;
}