#include"MeshObject.h"

MeshObject::MeshObject(MeshModel* model) {
	this->drawModel = model;
}
MeshObject::~MeshObject() {
	if (this->drawModel) {
		delete this->drawModel;
	}
}

void MeshObject::Draw(GameParameters& param, Vector3f& position, Vector3f& rotation, Vector3f& scale) {
	SetMatrix(param, position, rotation, scale);
	this->drawModel->Draw(param);
}
void MeshObject::InstanceDraw(GameParameters& param, int numDraw, Vector3f& position, Vector3f& rotation, Vector3f& scale) {
	SetMatrix(param, position, rotation, scale);
	this->drawModel->InstanceDraw(numDraw, param);
}

////////
//protected
////

void MeshObject::SetMatrix(GameParameters& param, Vector3f& position, Vector3f& rotation, Vector3f& scaling) {
	//à⁄ìÆ
	Translation<float, 3> trans = Translation<float, 3>(position);
	//âÒì]èáÇÕYXZ
	Quaternionf rot;
	rot = AngleAxisf(0, Vector3f::Zero());
	rot = AngleAxisf(rotation.y(), Vector3f::UnitY()) * rot;
	rot = AngleAxisf(rotation.x(), Vector3f::UnitX()) * rot;
	rot = AngleAxisf(rotation.z(), Vector3f::UnitZ()) * rot;
	//ÉXÉPÅ[Éã
	DiagonalMatrix<float, 3> scale = DiagonalMatrix<float, 3>(Vector3f(-scaling.x(), scaling.y(), scaling.z()));


	Affine3f world = trans * rot * scale;
	Matrix4f view = param.camera->GetViewMatrix();
	Matrix4f projection = param.camera->GetProjectionMatrix();

	param.currentShader->SetMatrix(projection * view * world.matrix());
	param.currentShader->SetWorldMatrix(world.matrix());
}