#pragma once
#include"TextureList.h"
#include"ShaderList.h"
#include"LightList.h"
#include"CameraList.h"
#include"InputGLFW.h"
#include"BulletPhysics.h"
#include"FontRenderer.h"

#include"SoundClass.h"
#include"EffectClass.h"


//�`�搢�E�p�����[�^�[
//�E���������A���ׂẴI�u�W�F�N�g�ɋ��ʂȃf�[�^���ʂɓn���̂��A�z�炵���Ȃ��Ă����̂ł܂Ƃ߂�
//�E���̍\���̂����n���΁A�K�v�ȃf�[�^�̋��ʍ��ɂ��ׂăA�N�Z�X�ł���
//�E�������A����A�z�Q�Ƃɂ͋C�����Ă�
class GameParameters {
public:
	GameParameters(GLFWwindow* window, int screenWidth, int screenHeight);
	~GameParameters();
	bool Initialize(GLFWwindow* window, int screenWidth, int screenHeight);
	void Finalize();
	void Run();

	void UseAmbient(const std::string& name);
	void UseDirectional(const std::string& name);
	void UseShader(const std::string& name);
	void UseCamera(const std::string& name);


	K_Input::InputGLFW*    GetInput();
	K_Physics::BulletPhysics* GetPhysics();
	K_Graphics::TextureList*   GetTextureList();
	K_Graphics::ShaderList*    GetShaderList();
	K_Graphics::LightList*     GetLightList();
	K_Graphics::CameraList*    GetCameraList();
	K_Audio::SoundClass*    GetAudioList();
	K_Graphics::EffectClass*   GetEffects();
	K_Graphics::FontRenderer*  GetFontRenderer();

private:
	K_Input::InputGLFW *    input;
	K_Physics::BulletPhysics* physicsSystem;
	K_Graphics::TextureList*   textureList;
	K_Graphics::ShaderList*    shaderList;
	K_Graphics::LightList*     lightList;
	K_Graphics::CameraList*    cameraList;
	K_Audio::SoundClass*    audioPlayer;
	K_Graphics::EffectClass*   effectSystem;
	K_Graphics::FontRenderer*  fontRenderer;

public:
	K_Graphics::CameraClass*   currentCamera;
	K_Graphics::ShaderClass*   currentShader;
	int            screenWidth;
	int            screenHeight;
};