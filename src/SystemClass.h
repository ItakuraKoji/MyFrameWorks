#pragma once

#define GLEW_STATIC

#if defined(_DEBUG)
#pragma comment(lib, "glew32d.lib")
#pragma comment(lib, "glfw3d.lib")
#pragma comment(lib, "BulletDynamics_Debug.lib")
#pragma comment(lib, "BulletCollision_Debug.lib")
#pragma comment(lib, "LinearMath_Debug.lib")
#else
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "BulletDynamics.lib")
#pragma comment(lib, "BulletCollision.lib")
#pragma comment(lib, "LinearMath.lib")
#endif
#pragma comment(lib, "OPENGL32.lib")
#pragma comment(lib, "libfbxsdk-md.lib")

#pragma comment(lib, "winmm.lib")//時間計測用


#include<Windows.h>
#include<GLEW/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include"MyApplication.h"
#include"InputClass.h"


//アプリケーションの動作を管理
class SystemClass {
public:
	SystemClass();
	~SystemClass();
	bool Initialize();
	void Run();

private:
	bool CreateAppricationWindow(const char* windowName, int width, int height, bool fullScreen);
	bool InitializeOpenGL();
	bool InitializeApplication();
	
private:
	int screenWidth, screenHeight;
	float screenNeer, screenFar;
	bool isFullScreen;
	GLFWwindow* windowHandle;
	MyApplication* application;

	//FPS計測
	unsigned long cullentTime;
	unsigned long startTime;
	int framecount;
};