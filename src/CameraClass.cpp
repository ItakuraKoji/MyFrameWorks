#include"CameraClass.h"

CameraClass::CameraClass(CameraType type, int width, int height, float near, float far, float fov) {
	SetPosition(0, 0, 0);
	SetTarget(0, 0, 0);
	this->screenWidth = width;
	this->screenHeight = height;
	this->screenNear = near;
	this->screenFar = far;
	this->fieldOfView = fov;
	this->projectionType = type;
	Draw();
}
CameraClass::~CameraClass(){

}

const Matrix4f& CameraClass::GetViewMatrix(){
	return this->viewMatrix;

}
const Matrix4f& CameraClass::GetCameraMatrix() {
	return this->cameraMatrix;
}
const Matrix4f& CameraClass::GetProjectionMatrix() {
	return this->projectionMatrix;
}

void CameraClass::SetPosition(float x, float y, float z) {
	this->position << x, y, z;
}


void CameraClass::SetTarget(float x, float y, float z) {
	this->target << x, y, z;
}

const Vector3f& CameraClass::GetAxisX() {
	return this->xAxis;
}
const Vector3f& CameraClass::GetAxisY() {
	return this->yAxis;
}
const Vector3f& CameraClass::GetAxisZ() {
	return this->zAxis;
}
const Vector3f& CameraClass::GetTerget() {
	return this->target;
}
const Vector3f& CameraClass::GetPosition() {
	return this->position;
}

int CameraClass::GetScreenWidth() {
	return this->screenWidth;
}
int CameraClass::GetScreenHeight() {
	return this->screenHeight;
}
float CameraClass::GetScreenNear() {
	return this->screenNear;
}
float CameraClass::GetScreenFar() {
	return screenFar;
}
float CameraClass::GetFieldOfView() {
	return this->fieldOfView;
}

//注視点を与えてビュー行列を作成
void CameraClass::Draw() {
	Vector3f up;
	//カメラの上方向を定義
	up.x() = 0.0f;
	up.y() = 1.0f;
	up.z() = 0.0f;
	M::MatrixLookAt(this->viewMatrix, this->position, this->target, up);

	//逆行列（カメラ行列）も保持
	this->cameraMatrix = this->viewMatrix;
	this->cameraMatrix.block(0, 0, 3, 3) = this->viewMatrix.block(0, 0, 3, 3).transpose();
	this->cameraMatrix.block(3, 0, 1, 3) = -this->viewMatrix.block(3, 0, 1, 3);

	//カメラ軸
	this->xAxis = this->viewMatrix.block(0, 0, 1, 3).transpose();
	this->yAxis = this->viewMatrix.block(1, 0, 1, 3).transpose();
	this->zAxis = this->viewMatrix.block(2, 0, 1, 3).transpose();

	//プロジェクション行列を作成
	if (this->projectionType == CameraType::Perspective) {
		M::MatrixPerspectiveLH(this->projectionMatrix, this->screenWidth, this->screenHeight, this->screenNear, this->screenFar, this->fieldOfView);
	}
	else {
		M::MatrixOrthoLH(this->projectionMatrix, this->screenWidth, this->screenHeight, this->screenNear, this->screenFar);
	}
}
