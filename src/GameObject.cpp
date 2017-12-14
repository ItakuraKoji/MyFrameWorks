#include"GameObject.h"

GameObject::GameObject() {
	this->position = Vector3f(0, 0, 0);
	this->rotation = Vector3f(0, 0, 0);
	this->scale    = Vector3f(1, 1, 1);

	this->drawModel = 0;
}
GameObject::~GameObject() {
	if (this->drawModel) {
		delete this->drawModel;
	}
}

void GameObject::SetDrawModel(MeshModel* model) {
	if (this->drawModel) {
		delete this->drawModel;
	}
	this->drawModel = model;
}
void GameObject::SetShaderName(const std::string& name) {
	this->shaderName = name;
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

MeshModel* GameObject::GetModel() {
	return GameObject::drawModel;
}

void GameObject::SetMatrix(GameParameters& param) {


	//à⁄ìÆ
	Translation<float, 3> trans = Translation<float, 3>(this->position);
	//âÒì]èáÇÕYXZ
	Quaternionf rot;
	rot = AngleAxisf(0, Vector3f::Zero());
	rot = AngleAxisf(DegToRad(this->rotation.y()), Vector3f::UnitY()) * rot;
	rot = AngleAxisf(DegToRad(this->rotation.x()), Vector3f::UnitX()) * rot;
	rot = AngleAxisf(DegToRad(this->rotation.z()), Vector3f::UnitZ()) * rot;
	//ÉXÉPÅ[Éã
	DiagonalMatrix<float, 3> scale(this->scale);

	trans = Translation<float, 3>(this->position.x(), this->position.y(), this->position.z());
	scale = DiagonalMatrix<float, 3>(-1.0f, 1.0f, 1.0f);

	rot = AngleAxisf(0, Vector3f::Zero());
	rot = AngleAxisf(DegToRad(-90.0f), Vector3f::UnitX()) * rot;
	rot = AngleAxisf(this->rotation.y(), Vector3f(0.0f, 1.0f, 0.0f)) * rot;

	Affine3f world      = trans * rot * scale;
	Matrix4f view       = param.camera->GetViewMatrix();
	Matrix4f projection = param.camera->GetProjectionMatrix();

	ShaderClass* shader = param.shaderList->GetShader(this->shaderName);
	shader->SetMatrix(projection * view * world.matrix());
	shader->SetWorldMatrix(world.matrix());
}