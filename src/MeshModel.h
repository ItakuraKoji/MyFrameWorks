#pragma once

#include"FbxModelLoader.h"
#include"ModelData.h"
#include"DrawParameters.h"
#include"ShaderClass.h"


//���f���N���X�B�t�@�N�g���[���琶�Y���ꂽ���f���f�[�^���󂯎���ď�����
//�u�`��v�Ƃ��������Ȃ̂ŁA�`�掞�̓��f���f�[�^�ȊO�̕K�v�ȏ����O����n�����̂Ƃ���
class MeshModel {
public:
	MeshModel(ModelDatas* data);
	~MeshModel();

	bool Initialize(ModelDatas* data);
	void Finalize();
	void BindVAO();
	void SetAnimation(const std::string& animationName, bool isLoop, bool isInterporation, bool playOnce);
	void SetSpeed(int speed);
	void Draw(DrawParameters& param, const std::string& shaderName);
	void InstanceDraw(int numInstance, DrawParameters& param, const std::string& shaderName);

private:
	void SetBone    (int arrayIndex, DrawParameters& param, const std::string& shaderName);
	void DrawBuffers(int arrayIndex, DrawParameters& param, const std::string& shaderName);

private:
	ModelDatas* data;
	Texture*  boneTexture;

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};