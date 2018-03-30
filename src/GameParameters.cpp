#include"GameParameters.h"

GameParameters::GameParameters(GLFWwindow* window, int screenWidth, int screenHeight) {
	if (!Initialize(window, screenWidth, screenHeight)) {
		Finalize();
		throw;
	}
}
GameParameters::~GameParameters() {
	Finalize();
}
bool GameParameters::Initialize(GLFWwindow* window, int screenWidth, int screenHeight) {
	Finalize();
	try {
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		this->physicsSystem = new K_Physics::BulletPhysics;
		this->input = new K_Input::InputGLFW(0, window);
		this->textureList = new K_Graphics::TextureList;
		this->shaderList = new K_Graphics::ShaderList;
		this->cameraList = new K_Graphics::CameraList;
		this->lightList = new K_Graphics::LightList;
		this->audioPlayer = new K_Audio::SoundClass;
		this->effectSystem = new K_Graphics::EffectClass;
		this->fontRenderer = new K_Graphics::FontRenderer;
	}
	catch(...){
		return false;
	}
	return true;
}
void GameParameters::Finalize() {
	if (this->input != nullptr) {
		delete this->input;
		this->input = nullptr;
	}
	if (this->physicsSystem != nullptr) {
		delete this->physicsSystem;
		this->physicsSystem = nullptr;
	}
	if (this->textureList != nullptr) {
		delete this->textureList;
		this->textureList = nullptr;
	}
	if (this->shaderList != nullptr) {
		delete this->shaderList;
		this->shaderList = nullptr;
	}
	if (this->lightList != nullptr) {
		delete this->lightList;
		this->lightList = nullptr;
	}
	if (this->cameraList != nullptr) {
		delete this->cameraList;
		this->cameraList = nullptr;
	}
	if (this->audioPlayer != nullptr) {
		delete this->audioPlayer;
		this->audioPlayer = nullptr;
	}
	if (this->effectSystem != nullptr) {
		delete this->effectSystem;
		this->effectSystem = nullptr;
	}
	if (this->fontRenderer != nullptr) {
		delete this->fontRenderer;
		this->fontRenderer = nullptr;
	}
}
void GameParameters::Run() {
	this->input->Run();
	this->physicsSystem->Run();
	this->effectSystem->Run();
}

void GameParameters::UseAmbient(const std::string& name) {
	this->lightList->SetAmbient(name, this->currentShader);
}
void GameParameters::UseDirectional(const std::string& name) {
	this->lightList->SetDirectional(name, this->currentShader);
}
void GameParameters::UseShader(const std::string& name) {
	this->currentShader = this->shaderList->UseShader(name);
}
void GameParameters::UseCamera(const std::string& name) {
	this->currentCamera = this->cameraList->GetCamera(name);
	if (this->currentShader) {
		this->currentShader->SetValue("cameraPos", this->currentCamera->GetPosition());
	}
}

K_Input::InputGLFW* GameParameters::GetInput() {
	return this->input;
}
K_Physics::BulletPhysics* GameParameters::GetPhysics() {
	return this->physicsSystem;
}
K_Graphics::TextureList* GameParameters::GetTextureList() {
	return this->textureList;
}
K_Graphics::ShaderList* GameParameters::GetShaderList() {
	return this->shaderList;
}
K_Graphics::LightList* GameParameters::GetLightList() {
	return this->lightList;
}
K_Graphics::CameraList* GameParameters::GetCameraList() {
	return this->cameraList;
}
K_Audio::SoundClass* GameParameters::GetAudioList() {
	return this->audioPlayer;
}
K_Graphics::EffectClass* GameParameters::GetEffects() {
	return this->effectSystem;
}
K_Graphics::FontRenderer* GameParameters::GetFontRenderer() {
	return this->fontRenderer;
}



