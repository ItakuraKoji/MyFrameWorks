#pragma once

#include"MeshModel.h"
#include"GameParameters.h"
#include"MyMathFanctions.h"

#include<Eigen\Core>
#include<Eigen\Geometry>
#include<string>

//�Q�[���ɂ����镨�̊��N���X�A�h�������ăL�����N�^�̋���������������
//���q����邱�Ƃ�������͉\
//�E�ʒu�A��]�A�g�k
//�E�`��Ɏg�p����V�F�[�_�[�̖��O
//�E�`�惂�f��(����ӔC������)
class GameObject {
public:
	GameObject();
	virtual ~GameObject();
	virtual void Run(GameParameters* param) = 0;
	virtual void Draw(GameParameters* param) = 0;

	void SetDrawModel(K_Graphics::MeshObject* model);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	K_Math::Vector3 GetPosition();
	K_Math::Vector3 GetRotation();
	K_Math::Vector3 GetScale();

protected:
	K_Graphics::MeshObject* GetModel();
	void LoadModel(GameParameters& param);

protected:
	K_Math::Vector3        position;
	K_Math::Vector3        rotation;
	K_Math::Vector3        scale;
private:
	//��Ȃ��̂ŐG�点�Ȃ����j��
	K_Graphics::MeshObject*      drawModel;
};