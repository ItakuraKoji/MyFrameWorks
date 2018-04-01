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
SystemClass::SystemClass(int windowWidth, int windowHeight, bool isFullScreen) {
	if (!Initialize(windowWidth, windowHeight, isFullScreen)) {
		Finalize();
		throw("system Initialize Failed");
	}
}
SystemClass::~SystemClass() {
	Finalize();
}

bool SystemClass::Initialize(int windowWidth, int windowHeight, bool isFullScreen) {
	Finalize();

	this->screenWidth = windowWidth;
	this->screenHeight = windowHeight;
	this->isFullScreen = isFullScreen;
	this->windowHandle = nullptr;
	this->parameters = nullptr;
	this->windowClosed = false;
	this->fps = 0.0f;

	if (!this->CreateAppricationWindow("OpenGL", this->screenWidth, this->screenHeight, this->isFullScreen)) {
		return false;
	}
	if (!this->InitializeOpenGL()) {
		return false;
	}

	this->startTime = std::chrono::system_clock::now();
	this->framecount = 0;
	return true;
}
void SystemClass::Finalize() {
	if (this->parameters != nullptr) {
		delete this->parameters;
		this->parameters = nullptr;
	}
	glfwTerminate();
}

void SystemClass::ProcessSystem() {
	//関数が０を返したときはウィンドウは閉じられていない
	this->windowClosed = (bool)glfwWindowShouldClose(this->windowHandle);

	glfwPollEvents();
	//関数が０を返したときはウィンドウはフォーカスされていない
	this->isFocus = (bool)glfwGetWindowAttrib(this->windowHandle, GLFW_FOCUSED);

	if (!this->isFocus) {
		//FPS計測はフォーカスしているときだけ
		return;
	}

	//FPS計測
	std::chrono::system_clock::time_point a = std::chrono::system_clock::now();

	++this->framecount;
	if (this->framecount == 60) {
		cullentTime = std::chrono::system_clock::now();
		this->fps = (float)framecount / std::chrono::duration_cast<std::chrono::milliseconds>(cullentTime - startTime).count() * 1000;
		this->startTime = std::chrono::system_clock::now();

		std::cout << this->fps << std::endl;
		this->framecount = 0;
	}
}

void SystemClass::SwapBuffer() {
	glfwSwapBuffers(this->windowHandle);
}

bool SystemClass::WindowForcus() {
	return this->isFocus;
}
bool SystemClass::WindowClosed() {
	return this->windowClosed;
}

int SystemClass::GetWindowWidth() {
	return this->screenWidth;
}
int SystemClass::GetWindowHeight() {
	return this->screenHeight;
}
GameParameters* SystemClass::GetParameters() {
	return this->parameters;
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

	//ゲームのためのシステムを初期化
	try {
		this->parameters = new GameParameters(this->windowHandle, this->screenWidth, this->screenHeight);
	}
	catch (...) {
		return false;
	}
	return true;
}

