#pragma once
#include<string>
#include"DrawParameters.h"
#include"ModelData.h"
#include"FbxModelLoader.h"

//���f���N���X�̏������ɕK�v�ȃp�����[�^�[�̐����S������N���X
//�E�O���t�@�C���̓ǂݍ���
//�E��{�}�`(�|���S���Ȃ�)
class ModelDataFactory {
public:
	ModelDatas* LoadFBXModel(const std::string& fileName, DrawParameters& param);
	ModelDatas* CreateSquareModel(const std::string& textureName, DrawParameters& param);
};