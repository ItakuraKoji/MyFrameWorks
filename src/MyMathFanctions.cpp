#include"MyMathFanctions.h"

float DegToRad(float deg) {
	return deg * (float)M_PI / 180.0f;
}
float RadToDeg(float rad) {
	return rad * 180.0f / (float)M_PI;
}

void MatrixPerspectiveLH(Eigen::Matrix4f& result, float screenWidth, float screenHeight, float screenNear, float screenFar, float fieldOfView) {
	Eigen::Matrix4f mat;
	mat(0) = screenHeight / (screenWidth * tanf(fieldOfView * 0.5f));
	mat(1) = 0.0f;
	mat(2) = 0.0f;
	mat(3) = 0.0f;

	mat(4) = 0.0f;
	mat(5) = 1.0f / tanf(fieldOfView * 0.5f);
	mat(6) = 0.0f;
	mat(7) = 0.0f;

	mat(8) = 0.0f;
	mat(9) = 0.0f;
	mat(10) = screenFar / (screenFar - screenNear);
	mat(11) = 1.0f;

	mat(12) = 0.0f;
	mat(13) = 0.0f;
	mat(14) = screenFar * -screenNear / (screenFar - screenNear);
	mat(15) = 0.0f;
	result = mat;
}

void MatrixPerspectiveRH(Eigen::Matrix4f& result, float screenWidth, float screenHeight, float screenNear, float screenFar, float fieldOfView) {

}
void MatrixOrthoLH(Eigen::Matrix4f& result, float screenWidth, float screenHeight, float screenNear, float screenFar) {
	Eigen::Matrix4f mat;
	mat(0) = 2.0f / screenWidth;
	mat(1) = 0.0f;
	mat(2) = 0.0f;
	mat(3) = 0.0f;

	mat(4) = 0.0f;
	mat(5) = 2.0f / screenHeight;
	mat(6) = 0.0f;
	mat(7) = 0.0f;

	mat(8) = 0.0f;
	mat(9) = 0.0f;
	mat(10) = 2.0f / (screenFar - screenNear);
	mat(11) = 0.0f;

	mat(12) = 0.0f;
	mat(13) = 0.0f;
	mat(14) = -(screenNear) / (screenFar - screenNear);
	mat(15) = 1.0f;

	result = mat;
}
void MatrixOrthoRH(Eigen::Matrix4f& result, float screenWidth, float screenHeight, float screenNear, float screenFar) {

}
void MatrixLookAt(Eigen::Matrix4f& result, Eigen::Vector3f& position, Eigen::Vector3f& lookAt, Eigen::Vector3f& up) {
	Eigen::Vector3f zAxis, xAxis, yAxis;

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

	// ÉrÉÖÅ[çsóÒÇçÏê¨
	Eigen::Matrix4f mat;
	mat << xAxis.x(), xAxis.y(), xAxis.z(), result1,
		   yAxis.x(), yAxis.y(), yAxis.z(), result2,
		   zAxis.x(), zAxis.y(), zAxis.z(), result3,
		   0.0f,      0.0f,      0.0f,      1.0f;
	result = mat;
}
