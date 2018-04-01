#pragma once

#define GLEW_STATIC

#pragma warning (disable : 4099)

#pragma comment(lib, "OPENGL32.lib")
#pragma comment(lib, "libfbxsdk-md.lib")

#pragma comment(lib, "common.lib")
#pragma comment(lib, "OpenAL32.lib")

#pragma comment ( lib, "libogg_static.lib" )
#pragma comment ( lib, "libvorbis_static.lib" )
#pragma comment ( lib, "libvorbisfile_static.lib" )

#pragma comment(lib, "Effekseer.lib" )
#pragma comment(lib, "EffekseerRendererGL.lib" )
#pragma comment(lib, "EffekseerSoundAL.lib" )

#pragma comment(lib, "freetype.lib" )

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

#include<Windows.h>
#include<iostream>
#include<chrono>

#include<GLEW/glew.h>
#include<GLFW/glfw3.h>

#include"GameParameters.h"
#include"MyApplication.h"

//アプリケーションの動作を管理
class SystemClass {
public:
	SystemClass(int windowWidth, int windowHeight, bool isFullScreen);
	~SystemClass();
	bool Initialize(int windowWidth, int windowHeight, bool isFullScreen);
	void Finalize();
	//ウィンドウイベント、システム関連の更新処理を行う
	void ProcessSystem();
	//バッファ入れ替えによって描画を反映
	void SwapBuffer();

	bool WindowForcus();
	bool WindowClosed();
	int GetWindowWidth();
	int GetWindowHeight();
	GameParameters* GetParameters();

private:
	bool CreateAppricationWindow(const char* windowName, int width, int height, bool fullScreen);
	bool InitializeOpenGL();
	
private:
	int screenWidth, screenHeight;
	bool isFullScreen;
	bool isFocus;
	bool windowClosed;
	GLFWwindow* windowHandle;
	GameParameters* parameters;

	//FPS計測
	float fps;
	std::chrono::system_clock::time_point cullentTime;
	std::chrono::system_clock::time_point startTime;
	int framecount;
};