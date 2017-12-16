#include"CameraMan.h"


bool CameraMan::Initialize(GameParameters& param) {
	return true;
}
void CameraMan::Finalize() {

}
void CameraMan::Run(GameParameters& param) {

	float srRotation = param.input->GetStickRotation(VPAD_STICK_R);
	float srPower = param.input->GetStickPower(VPAD_STICK_R);

	this->camerarotH += 2.0f * srPower * cosf(srRotation);
	this->camerarotV -= 2.0f * srPower * sinf(srRotation);

	float cx = -20.0f * sinf(camerarotH * (float)M_PI / 180.0f) + this->position.x();
	float cy = -20.0f * sinf(camerarotV * (float)M_PI / 180.0f) + this->position.y();
	float cz = -20.0f * cosf(camerarotH * (float)M_PI / 180.0f) * cosf(camerarotV * (float)M_PI / 180.0f) + this->position.z();

	this->camera->SetPosition(cx, cy + 4.0f, cz);
	this->camera->SetTarget(this->position.x(), this->position.y() + 4.0f, this->position.z());
}
void CameraMan::Draw(GameParameters& param) {

}

void CameraMan::SetParameter(GameObject* terget, CameraClass* camera) {
	this->terget = terget;
	this->camera = camera;
}
