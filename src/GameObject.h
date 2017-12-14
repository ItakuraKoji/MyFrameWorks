#pragma once

#include"MeshModel.h"
#include"DrawParameters.h"
#include"MyMathFanctions.h"

#include<Eigen\Core>
#include<Eigen\Geometry>
#include<string>

using namespace Eigen;

//�Q�[���ɂ����镨�̊��N���X�A�h�������ăL�����N�^�̋���������������
//���q����邱�Ƃ�������͉\
//�E�ʒu�A��]�A�g�k
//�E�`��Ɏg�p����V�F�[�_�[�̖��O
//�E�`�惂�f��(����ӔC������)
class GameObject {
public:
	GameObject();
	virtual ~GameObject();
	virtual bool Initialize(GameParameters& param) = 0;
	virtual void Finalize() = 0;
	virtual void Run(GameParameters& param) = 0;
	virtual void Draw(GameParameters& param) = 0;

	void SetDrawModel(MeshModel* model);
	void SetShaderName(const std::string& name);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	Vector3f GetPosition();
	Vector3f GetRotation();
	Vector3f GetScale();

protected:
	MeshModel* GetModel();
	void SetMatrix(GameParameters& param);

protected:
	Vector3f        position;
	Vector3f        rotation;
	Vector3f        scale;
	std::string     shaderName;
private:
	//��Ȃ��̂ŐG�点�Ȃ����j��
	MeshModel*      drawModel;
};