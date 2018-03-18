#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<Eigen\Core>
//‚Ç‚±‚É‚¨‚¢‚Ä‚à‚µ‚Á‚­‚è‚±‚È‚¢A‚»‚ñ‚ÈŽ©ì”ŠwŒnŠÖ”‚Ì—­‚Ü‚èê

namespace M {
	struct Box2D {
		Box2D() : x(0), y(0), w(0), h(0) {}
		Box2D(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
		Box2D& operator =(Box2D& box) { this->x = box.x; this->y = box.y; this->w = box.w; this->h = box.h; return *this; }
		int x, y, w, h;
	};

	float DegToRad(float deg);
	float RadToDeg(float rad);
	void  MatrixPerspectiveLH(Eigen::Matrix4f& result, float screenWidth, float screenHeight, float screenNear, float screenFar, float fieldOfView);
	void  MatrixPerspectiveRH(Eigen::Matrix4f& result, float screenWidth, float screenHeight, float screenNear, float screenFar, float fieldOfView);
	void  MatrixOrthoLH(Eigen::Matrix4f& result, float screenWidth, float screenHeight, float screenNear, float screenFar);
	void  MatrixOrthoRH(Eigen::Matrix4f& result, float screenWidth, float screenHeight, float screenNear, float screenFar);
	void  MatrixLookAt(Eigen::Matrix4f& result, Eigen::Vector3f& position, Eigen::Vector3f& lookAt, Eigen::Vector3f& up);
}
