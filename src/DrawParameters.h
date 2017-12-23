#pragma once
#include"TextureList.h"
#include"ShaderList.h"
#include"LightList.h"
#include"CameraClass.h"
#include"InputClass.h"
#include"BulletPhysics.h"

//�`�搢�E�p�����[�^�[
//�E���������A���ׂẴI�u�W�F�N�g�ɋ��ʂȃf�[�^���ʂɓn���̂��A�z�炵���Ȃ��Ă����̂ł܂Ƃ߂�
//�E���̍\���̂����n���΁A�K�v�ȃf�[�^�̋��ʍ��ɂ��ׂăA�N�Z�X�ł���
class GameParameters {
public:
	InputClass*    input;
	BulletPhysics* physicsSystem;
	TextureList*   textureList;
	ShaderList*    shaderList;
	LightList*     lightList;
	CameraClass*   camera;
	ShaderClass*   currentShader;
	int            screenWidth;
	int            screenHeight;
	float          screenFov;
	float          gravity;
};