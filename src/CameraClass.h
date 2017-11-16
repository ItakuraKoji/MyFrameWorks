#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<Eigen\Core>

using namespace Eigen;

class CameraClass {
public:
	CameraClass();
	~CameraClass();

	Matrix4f& GetViewMatrix();
	Matrix4f& GetViewMatrixInverse();
	void SetPosition(float x, float y, float z);
	void Draw();

	void SetTarget(float x, float y, float z);

	Vector3f GetAxisX();
	Vector3f GetAxisY();
	Vector3f GetAxisZ();
	Vector3f GetTerget();

private:
	//ÉrÉÖÅ[çsóÒÇçÏê¨
	void MatrixLookAt(Vector3f position, Vector3f lookAt, Vector3f up);

private:
	Vector3f target;
	Vector3f position;

	Matrix4f viewMatrix;
	Matrix4f viewMatrixInverse;
	Vector3f xAxis;
	Vector3f yAxis;
	Vector3f zAxis;

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
