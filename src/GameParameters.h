#pragma once
#include"TextureList.h"
#include"ShaderList.h"
#include"LightList.h"
#include"CameraList.h"
#include"InputClass.h"
#include"BulletPhysics.h"

#include"SoundClass.h"
#include"EffectClass.h"

//�`�搢�E�p�����[�^�[
//�E���������A���ׂẴI�u�W�F�N�g�ɋ��ʂȃf�[�^���ʂɓn���̂��A�z�炵���Ȃ��Ă����̂ł܂Ƃ߂�
//�E���̍\���̂����n���΁A�K�v�ȃf�[�^�̋��ʍ��ɂ��ׂăA�N�Z�X�ł���
//�E�������A����A�z�Q�Ƃɂ͋C�����Ă�
class GameParameters {
public:
	GameParameters();
	~GameParameters();
	bool Initialize(int screenWidth, int screenHeight);
	void Finalize();
	void Run();

	void UseAmbient(const std::string& name);
	void UseDirectional(const std::string& name);
	void UseShader(const std::string& name);
	void UseCamera(const std::string& name);


	InputClass*    GetInput();
	BulletPhysics* GetPhysics();
	TextureList*   GetTextureList();
	ShaderList*    GetShaderList();
	LightList*     GetLightList();
	CameraList*    GetCameraList();
	SoundClass*    GetAudioList();
	EffectClass*   GetEffects();

private:
	InputClass*    input;
	BulletPhysics* physicsSystem;
	TextureList*   textureList;
	ShaderList*    shaderList;
	LightList*     lightList;
	CameraList*    cameraList;
	SoundClass*    audioPlayer;
	EffectClass*   effectSystem;

public:
	CameraClass*   currentCamera;
	ShaderClass*   currentShader;
	int            screenWidth;
	int            screenHeight;
};