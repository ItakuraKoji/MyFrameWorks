#pragma once

#include"FbxModelLoader.h"
#include"ModelData.h"
#include"CameraClass.h"
#include"ShaderClass.h"

//���f���N���X�B�t�@�N�g���[���琶�Y���ꂽ���f���f�[�^���󂯎���ď�����
//�u�`��v�Ƃ��������Ȃ̂ŁA�`�掞�̓��f���f�[�^�ȊO�̕K�v�ȏ����O����n�����̂Ƃ���
class MeshModel {
public:
	MeshModel(ModelDatas* data);
	~MeshModel();

	bool Initialize(ModelDatas* data);
	void Finalize();
	void SetAnimation(const std::string& animationName, bool playOnce, bool isLoop, bool isInterporation, int interpolationFrames);
	void SetTexture(Texture* texture, int arrayIndex, int materialIndex);
	void SetSpeed(float speed);



	void UpdateAnimation();
	void Draw(ShaderClass* shader);
	void InstanceDraw(int numInstance, ShaderClass* shader);

private:
	void SetBone    (int arrayIndex, ShaderClass* shader);
	void DrawBuffers(int arrayIndex, ShaderClass* shader);

private:
	ModelDatas* data;
	Texture*  boneTexture;
	bool isBoneProcessed;

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};