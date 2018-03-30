#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<Eigen\Core>

//êîäwånÇÕÇ›ÇÒÇ»Ç±ÇÃñºëOãÛä‘Ç™Ç¢Ç¢Ç»Ç†Ç¡Çƒ
namespace K_Math {
	typedef Eigen::Matrix<float, 2, 1> Vector2;
	typedef Eigen::Matrix<float, 3, 1> Vector3;
	typedef Eigen::Matrix<float, 4, 1> Vector4;
	typedef Eigen::Matrix<float, 3, 3> Matrix3x3;
	typedef Eigen::Matrix<float, 4, 4> Matrix4x4;
	typedef Eigen::Quaternion<float> Quaternion;
	typedef Eigen::AngleAxis<float> AngleAxis;
	typedef Eigen::Translation<float, 3> Translation;
	typedef Eigen::DiagonalMatrix<float, 3> DiagonalMatrix;
	typedef Eigen::Transform<float, 3, 2> Affine3;

	struct Box2D {
		Box2D() : x(0), y(0), w(0), h(0) {}
		Box2D(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
		Box2D& operator =(Box2D& box) { this->x = box.x; this->y = box.y; this->w = box.w; this->h = box.h; return *this; }
		int x, y, w, h;
	};

	float DegToRad(float deg);
	float RadToDeg(float rad);
	void  MatrixPerspectiveLH(K_Math::Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar, float fieldOfView);
	void  MatrixPerspectiveRH(K_Math::Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar, float fieldOfView);
	void  MatrixOrthoLH(K_Math::Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar);
	void  MatrixOrthoRH(K_Math::Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar);
	void  MatrixLookAt(K_Math::Matrix4x4& result, K_Math::Vector3& position, K_Math::Vector3& lookAt, K_Math::Vector3& up);
}
