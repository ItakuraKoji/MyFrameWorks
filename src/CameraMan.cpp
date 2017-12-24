#include"CameraMan.h"


bool CameraMan::Initialize(GameParameters* param) {
	this->camerarotH = 0.0f;
	this->camerarotV = 0.0f;
	this->distance = 20.0f;
	this->height = 4.0f;
	return true;
}
void CameraMan::Finalize() {

}
void CameraMan::Run(GameParameters* param) {

	float srRotation = param->GetInput()->GetStickRotation(VPAD_STICK_R);
	float srPower = param->GetInput()->GetStickPower(VPAD_STICK_R);

	this->camerarotH += 2.0f * srPower * cosf(srRotation);
	this->camerarotV -= 2.0f * srPower * sinf(srRotation);

	this->distance = 20.0f;
	this->height = 4.0f;

	float cx = -this->distance * sinf(DegToRad(camerarotH)) + this->terget->GetPosition().x();
	float cy = -this->distance * sinf(DegToRad(camerarotV)) + this->terget->GetPosition().y();
	float cz = -this->distance * cosf(DegToRad(camerarotH)) * cosf(DegToRad(camerarotV)) + this->terget->GetPosition().z();

	this->camera->SetPosition(cx, cy + this->height, cz);
	this->camera->SetTarget(this->terget->GetPosition().x(), this->terget->GetPosition().y() + this->height, this->terget->GetPosition().z());
}
void CameraMan::Draw(GameParameters* param) {

}

void CameraMan::SetParameter(GameObject* terget, CameraClass* camera) {
	this->terget = terget;
	this->camera = camera;
}

float CameraMan::GetRotationH() {
	return DegToRad(this->camerarotH);
}
float CameraMan::GetRotationV() {
	return DegToRad(this->camerarotV);
}