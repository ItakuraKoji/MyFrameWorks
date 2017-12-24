#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<Eigen\Core>
#include<iostream>
#include"MyMathFanctions.h"

enum CameraType {
	Perspective,
	Ortho,
};

using namespace Eigen;

class CameraClass {
public:
	CameraClass(CameraType type, int width, int height, float near, float far, float fov);
	~CameraClass();


	void Draw();
	const Matrix4f& GetViewMatrix();
	const Matrix4f& GetCameraMatrix();
	const Matrix4f& GetProjectionMatrix();

	void SetPosition(float x, float y, float z);
	void SetTarget(float x, float y, float z);

	const Vector3f& GetAxisX();
	const Vector3f& GetAxisY();
	const Vector3f& GetAxisZ();
	const Vector3f& GetTerget();
	const Vector3f& GetPosition();

	void SetScreenWidth(int width);
	void SetScreenHeight(int width);
	void SetScreenNear(float width);
	void SetScreenFar(float width);
	void SetFieldOfView(float width); 
	
	int   GetScreenWidth();
	int   GetScreenHeight();
	float GetScreenNear();
	float GetScreenFar();
	float GetFieldOfView();

private:
	Vector3f target;
	Vector3f position;

	Matrix4f viewMatrix;
	Matrix4f cameraMatrix;
	Vector3f xAxis;
	Vector3f yAxis;
	Vector3f zAxis;

	CameraType projectionType;
	Matrix4f       projectionMatrix;
	int            screenWidth;
	int            screenHeight;
	float          screenNear;
	float          screenFar;
	float          fieldOfView;


public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
