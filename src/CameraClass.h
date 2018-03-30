#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<Eigen\Core>
#include<iostream>
#include"MyMathFanctions.h"

namespace K_Graphics {

	enum CameraType {
		Perspective,
		Ortho,
	};

	class CameraClass {
	public:
		CameraClass(CameraType type, int width, int height, float near, float far, float fov);
		~CameraClass();


		void Draw();
		const K_Math::Matrix4x4& GetViewMatrix();
		const K_Math::Matrix4x4& GetCameraMatrix();
		const K_Math::Matrix4x4& GetProjectionMatrix();

		void SetPosition(float x, float y, float z);
		void SetTarget(float x, float y, float z);

		const K_Math::Vector3& GetAxisX();
		const K_Math::Vector3& GetAxisY();
		const K_Math::Vector3& GetAxisZ();
		const K_Math::Vector3& GetTerget();
		const K_Math::Vector3& GetPosition();

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
		K_Math::Vector3 target;
		K_Math::Vector3 position;

		K_Math::Matrix4x4 viewMatrix;
		K_Math::Matrix4x4 cameraMatrix;
		K_Math::Vector3 xAxis;
		K_Math::Vector3 yAxis;
		K_Math::Vector3 zAxis;

		CameraType projectionType;
		K_Math::Matrix4x4       projectionMatrix;
		int            screenWidth;
		int            screenHeight;
		float          screenNear;
		float          screenFar;
		float          fieldOfView;


	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};
}