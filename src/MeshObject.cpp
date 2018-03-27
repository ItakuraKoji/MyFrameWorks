#include"MeshObject.h"

MeshObject::MeshObject(MeshModel* model){
	if (model == nullptr) {
		throw("modelData is nullptr");
	}
	this->drawModel = model;
}
MeshObject::~MeshObject() {
	if (this->drawModel != nullptr) {
		delete this->drawModel;
		this->drawModel = nullptr;
	}
}

void MeshObject::SetBoneAnimation(const std::string& animationName, bool playOnce, bool isLoop, bool isInterporation, int interpolationFrames) {
	this->drawModel->SetAnimation(animationName, playOnce, isLoop, isInterporation, interpolationFrames);
}
void MeshObject::SetSpeed(float speed) {
	this->drawModel->SetSpeed(speed);
}

void MeshObject::UpdateAnimation() {
	this->drawModel->UpdateAnimation();
}

void MeshObject::Draw(CameraClass* camera, ShaderClass* shader, Vector3f& position, Vector3f& rotation, Vector3f& scale) {
	SetMatrix(camera, shader, position, rotation, scale);
	this->drawModel->Draw(shader);
}
void MeshObject::InstanceDraw(CameraClass* camera, ShaderClass* shader, int numDraw, Vector3f& position, Vector3f& rotation, Vector3f& scale) {
	SetMatrix(camera, shader, position, rotation, scale);
	this->drawModel->InstanceDraw(numDraw, shader);
}

////////
//protected
////

void MeshObject::SetMatrix(CameraClass* camera, ShaderClass* shader, Vector3f& position, Vector3f& rotation, Vector3f& scaling) {
	//à⁄ìÆ
	Translation<float, 3> trans = Translation<float, 3>(position);
	//âÒì]èáÇÕYXZ
	Quaternionf rot;
	rot = AngleAxisf(0, Vector3f::Zero());
	rot = rot * AngleAxisf(rotation.y(), Vector3f::UnitY());
	rot = rot * AngleAxisf(rotation.x(), Vector3f::UnitX());
	rot = rot * AngleAxisf(rotation.z(), Vector3f::UnitZ());
	//ÉXÉPÅ[Éã
	DiagonalMatrix<float, 3> scale = DiagonalMatrix<float, 3>(Vector3f(-scaling.x(), scaling.y(), scaling.z()));


	Affine3f world = trans * rot * scale;
	Matrix4f view = camera->GetViewMatrix();
	Matrix4f projection = camera->GetProjectionMatrix();

	shader->SetMatrix(projection * view * world.matrix());
	shader->SetWorldMatrix(world.matrix());
}