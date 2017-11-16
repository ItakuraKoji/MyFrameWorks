#include"CameraClass.h"

CameraClass::CameraClass() {
	SetPosition(0, 0, 0);
	SetTarget(0, 0, 0);

}
CameraClass::~CameraClass(){

}

Matrix4f& CameraClass::GetViewMatrix(){
	return this->viewMatrix;

}
Matrix4f& CameraClass::GetViewMatrixInverse() {
	return this->viewMatrixInverse;
}

void CameraClass::SetPosition(float x, float y, float z) {
	this->position << x, y, z;
}


void CameraClass::SetTarget(float x, float y, float z) {
	this->target << x, y, z;
}

Vector3f CameraClass::GetAxisX() {
	return this->xAxis;
}
Vector3f CameraClass::GetAxisY() {
	return this->yAxis;
}
Vector3f CameraClass::GetAxisZ() {
	return this->zAxis;
}
Vector3f CameraClass::GetTerget() {
	return this->target;
}

//注視点を与えてビュー行列を作成
void CameraClass::Draw() {
	Vector3f up;
	//カメラの上方向を定義
	up.x() = 0.0f;
	up.y() = 1.0f;
	up.z() = 0.0f;
	MatrixLookAt(this->position, this->target, up);
}

//ビュー行列を作成
void CameraClass::MatrixLookAt(Vector3f position, Vector3f lookAt, Vector3f up) {
	Vector3f zAxis, xAxis, yAxis;
	float length;

	// zAxis = normal(lookAt - position)
	zAxis.x() = lookAt.x() - position.x();
	zAxis.y() = lookAt.y() - position.y();
	zAxis.z() = lookAt.z() - position.z();
	zAxis.normalize();

	// xAxis = normal(cross(up, zAxis))
	xAxis.x() = (up.y() * zAxis.z()) - (up.z() * zAxis.y());
	xAxis.y() = (up.z() * zAxis.x()) - (up.x() * zAxis.z());
	xAxis.z() = (up.x() * zAxis.y()) - (up.y() * zAxis.x());
	xAxis.normalize();

	// yAxis = cross(zAxis, xAxis)
	yAxis.x() = (zAxis.y() * xAxis.z()) - (zAxis.z() * xAxis.y());
	yAxis.y() = (zAxis.z() * xAxis.x()) - (zAxis.x() * xAxis.z());
	yAxis.z() = (zAxis.x() * xAxis.y()) - (zAxis.y() * xAxis.x());

	float result1, result2, result3;
	// -dot(xAxis, position)
	result1 = ((xAxis.x() * position.x()) + (xAxis.y() * position.y()) + (xAxis.z() * position.z())) * -1.0f;
	// -dot(yaxis, eye)
	result2 = ((yAxis.x() * position.x()) + (yAxis.y() * position.y()) + (yAxis.z() * position.z())) * -1.0f;
	// -dot(zaxis, eye)
	result3 = ((zAxis.x() * position.x()) + (zAxis.y() * position.y()) + (zAxis.z() * position.z())) * -1.0f;

	// ビュー行列を作成
	Matrix4f mat;
	mat << xAxis.x(), xAxis.y(), xAxis.z(), result1,
		   yAxis.x(), yAxis.y(), yAxis.z(), result2,
		   zAxis.x(), zAxis.y(), zAxis.z(), result3,
		   0.0f,      0.0f,      0.0f,      1.0f;
	this->viewMatrix = mat;
	//ビューの逆行列を作成
	mat << xAxis.x(), yAxis.x(), zAxis.x(), -result1,
		   xAxis.y(), yAxis.y(), zAxis.y(), -result2,
		   xAxis.z(), yAxis.z(), zAxis.z(), -result3,
		   0.0f,      0.0f,      0.0f,      1.0f;
	this->viewMatrixInverse = mat;

	//カメラ座標軸は保持しておく
	this->xAxis = xAxis;
	this->yAxis = yAxis;
	this->zAxis = zAxis;
}
