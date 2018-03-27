#include"CameraMan.h"


bool CameraMan::Initialize(float distance, float height) {
	this->camerarotH = 0.0f;
	this->camerarotV = 0.0f;
	this->distance = distance;
	this->height = height;
	return true;
}
void CameraMan::Finalize() {

}
void CameraMan::Run(GameParameters* param) {

	float srRotation = param->GetInput()->GetStickRotation(VPAD_STICK_R);
	float srPower = param->GetInput()->GetStickPower(VPAD_STICK_R);

	this->camerarotH += 1.5f * srPower * cosf(srRotation);
	this->camerarotV += 1.5f * srPower * sinf(srRotation);

	//ターゲットの位置の変化を取得し、やや遅れるように改変
	Vector3f move;
	move.x() = this->terget->GetPosition().x() - this->camera->GetTerget().x();
	move.y() = this->terget->GetPosition().y() - this->camera->GetTerget().y() + this->height;
	move.z() = this->terget->GetPosition().z() - this->camera->GetTerget().z();
	move = move * 0.4f;
	this->camera->SetTarget(this->camera->GetTerget().x() + move.x(), this->camera->GetTerget().y() + move.y(), this->camera->GetTerget().z() + move.z());

	//中心点から距離分引く
	float dis = this->distance + move.norm() * 3.0f;
	float cx = -dis * sinf(M::DegToRad(camerarotH)) + this->terget->GetPosition().x();
	float cy = -dis * sinf(M::DegToRad(camerarotV)) + this->terget->GetPosition().y() + this->height;
	float cz = -dis * cosf(M::DegToRad(camerarotH)) * cosf(M::DegToRad(camerarotV)) + this->terget->GetPosition().z();

	//位置についても同様
	move.x() = cx - this->camera->GetPosition().x();
	move.y() = cy - this->camera->GetPosition().y();
	move.z() = cz - this->camera->GetPosition().z();
	move = move * 0.4f;

	this->camera->SetPosition(this->camera->GetPosition().x() + move.x(), this->camera->GetPosition().y() + move.y(), this->camera->GetPosition().z() + move.z());
}
void CameraMan::Draw(GameParameters* param) {

}

void CameraMan::SetParameter(GameObject* terget, CameraClass* camera) {
	this->terget = terget;
	this->camera = camera;

	float cx = -this->distance * sinf(M::DegToRad(camerarotH)) + this->terget->GetPosition().x();
	float cy = -this->distance * sinf(M::DegToRad(camerarotV)) + this->terget->GetPosition().y() + this->height;
	float cz = -this->distance * cosf(M::DegToRad(camerarotH)) * cosf(M::DegToRad(camerarotV)) + this->terget->GetPosition().z();
	this->camera->SetPosition(cx, cy, cz);
	this->camera->SetTarget(this->terget->GetPosition().x(), this->terget->GetPosition().y() + this->height, this->terget->GetPosition().z());
}

void CameraMan::SetRotation(float horizontal, float vertical) {
	this->camerarotH = horizontal;
	this->camerarotV = vertical;
}


float CameraMan::GetRotationH() {
	return M::DegToRad(this->camerarotH);
}
float CameraMan::GetRotationV() {
	return M::DegToRad(this->camerarotV);
}