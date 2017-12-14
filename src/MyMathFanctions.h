#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<Eigen\Core>

//どこにおいてもしっくりこない、そんな自作数学系関数の溜まり場
float DegToRad(float deg);
float RadToDeg(float rad);
void  MatrixPerspectiveLH(Eigen::Matrix4f& result, float screenWidth, float screenHeight, float screenNear, float screenFar, float fieldOfView);
void  MatrixPerspectiveRH(Eigen::Matrix4f& result, float screenWidth, float screenHeight, float screenNear, float screenFar, float fieldOfView);
void  MatrixOrthoLH(Eigen::Matrix4f& result, float screenWidth, float screenHeight, float screenNear, float screenFar);
void  MatrixOrthoRH(Eigen::Matrix4f& result, float screenWidth, float screenHeight, float screenNear, float screenFar);
void  MatrixLookAt(Eigen::Matrix4f& result, Eigen::Vector3f& position, Eigen::Vector3f& lookAt, Eigen::Vector3f& up);
