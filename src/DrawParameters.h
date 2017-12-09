#pragma once
#include"TextureList.h"
#include"ShaderList.h"
#include"CameraClass.h"

//�`�搢�E�p�����[�^�[
//�E���������A���ׂẴI�u�W�F�N�g�ɋ��ʂȕ`��f�[�^��n���̂��A�z�炵���Ȃ��Ă����̂ł܂Ƃ߂�
//�E���̍\���̂����n���΁A�`��̋��ʍ��ɂ��ׂăA�N�Z�X�ł���
struct DrawParameters {
	TextureList* textureList;
	ShaderList*  shaderList;
	CameraClass* mainCamera;
	int          screenWidth;
	int          screenHeight;
	float        screenFov;
};