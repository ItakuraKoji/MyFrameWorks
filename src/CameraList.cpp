#include"CameraList.h"

CameraList::CameraList() {
	Initialize();
}
CameraList::~CameraList() {
	Initialize();
}
void CameraList::Initialize() {
	for (auto i : this->cameraList) {
		delete i.second;
	}
	this->cameraList.clear();
}

//パースペクティブ投影
void CameraList::AddPerspectiveCamera(const std::string& name, Vector3f& position, Vector3f& target, int width, int height, float neer, float far, float fieldOfView) {
	if (this->cameraList.find(name) != this->cameraList.end()) {
		return;
	}

	CameraClass* camera = new CameraClass(CameraType::Perspective, width, height, neer, far, fieldOfView);
	camera->SetPosition(position.x(), position.y(), position.z());
	camera->SetTarget(target.x(), target.y(), target.z());
	camera->Draw();
	this->cameraList[name] = camera;
}

//平行投影
void CameraList::AddOrthoCamera(const std::string& name, Vector3f& position, Vector3f& target, int width, int height, float neer, float far) {
	if (this->cameraList.find(name) != this->cameraList.end()) {
		return;
	}

	CameraClass* camera = new CameraClass(CameraType::Ortho, width, height, neer, far, 0.0f);
	camera->SetPosition(position.x(), position.y(), position.z());
	camera->SetTarget(target.x(), target.y(), target.z());
	camera->Draw();
	this->cameraList[name] = camera;
}

CameraClass* CameraList::GetCamera(const std::string& name) {
	if (this->cameraList.find(name) == this->cameraList.end()) {
		return 0;
	}
	return this->cameraList[name];
}
