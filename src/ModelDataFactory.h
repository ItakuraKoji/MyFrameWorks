#pragma once
#include<string>
#include"GameParameters.h"
#include"ModelData.h"
#include"FbxModelLoader.h"

//���f���N���X�̏������ɕK�v�ȃp�����[�^�[�̐����S������N���X
//�E�O���t�@�C���̓ǂݍ���
//�E��{�}�`(�|���S���Ȃ�)
class ModelDataFactory {
public:
	ModelDatas* LoadFBXModel(const std::string& fileName, GameParameters* param);
	ModelDatas* CreateSquareModel(float width, float height, const std::string& textureName, GameParameters* param);
};