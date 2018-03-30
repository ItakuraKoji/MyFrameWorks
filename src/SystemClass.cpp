#include"SystemClass.h"

//Escキーでアプリ終了
void GLFWKeyEvent(GLFWwindow* window, int key, int scanCode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

////////
//public
////
SystemClass::SystemClass() {
	this->screenWidth = 1920 / 2;
	this->screenHeight = 1080 / 2;
	this->isFullScreen = false;
	this->windowHandle = nullptr;
}
SystemClass::~SystemClass() {
	if (this->application != nullptr) {
		delete this->application;
		this->application = nullptr;
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
		//ウィンドウフォーカス時のみゲームを処理する
		if (!glfwGetWindowAttrib(this->windowHandle, GLFW_FOCUSED)) {
			continue;
		}

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

	GLFWmonitor *monitor = nullptr;
	//フルスクリーンの際はプライマリーモニターに出力する
	if (fullScreen) {
		monitor = glfwGetPrimaryMonitor();
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);


	this->windowHandle = glfwCreateWindow(width, height, windowName, monitor, nullptr);
	if (this->windowHandle == nullptr) {
		return false;
	}
	glfwGetFramebufferSize(this->windowHandle, &this->screenWidth, &this->screenHeight);
	glfwSetInputMode(this->windowHandle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetKeyCallback(this->windowHandle, GLFWKeyEvent);

	//ウィンドウサイズ変更を禁止
	glfwSetWindowSizeLimits(this->windowHandle, width, height, width, height);

	glfwMakeContextCurrent(this->windowHandle);
	glfwSwapInterval(1);
	
	return true;
}

bool SystemClass::InitializeOpenGL() {
	if (glewInit() != GLEW_OK) {
		return false;
	}
	
	//OpenGLの描画設定を初期化
	glClearDepth(1.0f);
	glClearStencil(0);

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	return true;
}

bool SystemClass::InitializeApplication() {
	this->application = new MyApplication;
	if (this->application == nullptr) {
		return false;
	}
	if (!this->application->Initialize(this->windowHandle, this->screenWidth, this->screenHeight)) {
		return false;
	}
	return true;
}



