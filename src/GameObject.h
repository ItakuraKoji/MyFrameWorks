#pragma once

#include"MeshObject.h"
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
	virtual void Run(GameParameters& param) = 0;
	virtual void Draw(GameParameters& param) = 0;

	void SetDrawModel(MeshObject* model);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	Vector3f GetPosition();
	Vector3f GetRotation();
	Vector3f GetScale();

protected:
	MeshObject* GetModel();
	void LoadModel(GameParameters& param);

protected:
	Vector3f        position;
	Vector3f        rotation;
	Vector3f        scale;
private:
	//��Ȃ��̂ŐG�点�Ȃ����j��
	MeshObject*      drawModel;
};