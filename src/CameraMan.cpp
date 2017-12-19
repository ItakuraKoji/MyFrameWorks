#include"CameraMan.h"


bool CameraMan::Initialize(GameParameters& param) {
	this->camerarotH = 0.0f;
	this->camerarotV = 0.0f;
	return true;
}
void CameraMan::Finalize() {

}
void CameraMan::Run(GameParameters& param) {

	float srRotation = param.input->GetStickRotation(VPAD_STICK_R);
	float srPower = param.input->GetStickPower(VPAD_STICK_R);

	this->camerarotH += 2.0f * srPower * cosf(srRotation);
	this->camerarotV -= 2.0f * srPower * sinf(srRotation);

	float cx = -20.0f * sinf(DegToRad(camerarotH)) + this->terget->GetPosition().x();
	float cy = -20.0f * sinf(DegToRad(camerarotV)) + this->terget->GetPosition().y();
	float cz = -20.0f * cosf(DegToRad(camerarotH)) * cosf(DegToRad(camerarotV)) + this->terget->GetPosition().z();

	this->camera->SetPosition(cx, cy + 4.0f, cz);
	this->camera->SetTarget(this->terget->GetPosition().x(), this->terget->GetPosition().y() + 4.0f, this->terget->GetPosition().z());
}
void CameraMan::Draw(GameParameters& param) {

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