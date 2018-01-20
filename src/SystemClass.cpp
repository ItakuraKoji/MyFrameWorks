#include"SystemClass.h"

////////
//public
////
SystemClass::SystemClass() {
	this->screenWidth = 600;
	this->screenHeight = 600;
	this->screenNeer = 0.1f;
	this->screenFar = 1000.0f;
	this->isFullScreen = false;
	this->windowHandle = NULL;
}
SystemClass::~SystemClass() {
	if (this->application) {
		delete this->application;
		this->application = NULL;
	}

	glfwTerminate();
}

bool SystemClass::Initialize() {
	if (!this->CreateAppricationWindow("OpenGL", this->screenWidth, this->screenHeight, this->isFullScreen)) {
		return false;
	}
	if (!this->InitializeOpenGL()) {
		return false;
	}
	if (!this->InitializeApplication()) {
		return false;
	}

	//分解能は1ミリ秒
	startTime = timeGetTime();
	framecount = 0;
	return true;
}


void SystemClass::Run() {
	while (!glfwWindowShouldClose(this->windowHandle))
	{
		glfwPollEvents();
		application->Run();
		application->Draw();
		glfwSwapBuffers(this->windowHandle);

		++framecount;

		if (framecount == 60) {
			cullentTime = timeGetTime();
			double fps = (double)(framecount) / (cullentTime - startTime) * 1000;
			startTime = timeGetTime();

			std::cout << fps << std::endl;
			framecount = 0;
		}
	}
}



////////
//private
////
bool SystemClass::CreateAppricationWindow(const char* windowName, int width, int height, bool fullScreen) {
	if (!glfwInit()) {
		return false;
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	//フルスクリーンの際はプライマリーモニターに出力する
	GLFWmonitor *monitor = NULL;
	if (fullScreen) {
		monitor = glfwGetPrimaryMonitor();
	}

	this->windowHandle = glfwCreateWindow(width, height, windowName, monitor, NULL);
	if (!this->windowHandle) {
		return false;
	}

	glfwMakeContextCurrent(this->windowHandle);
	glfwSwapInterval(1);
	
	return true;
}

bool SystemClass::InitializeOpenGL() {
	if (glewInit() != GLEW_OK) {
		return false;
	}

	//OpenGLの描画設定を初期化
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearDepth(1.0f);
	glClearStencil(0);

	glEnable(GL_DEPTH_TEST);
	return true;
}

bool SystemClass::InitializeApplication() {
	this->application = new MyApplication;
	if (!this->application) {
		return false;
	}
	if (!this->application->Initialize(this->screenWidth, this->screenHeight)) {
		return false;
	}
	return true;
}

