#pragma once
#include"MeshModel.h"

//���f���̕`�������܂Ƃ߂��N���X
//�ǂ��������Ă����ƁA�`�������MeshModel����荂���x���ň�������
class MeshObject{
public:
	MeshObject(MeshModel* model);
	~MeshObject();

	void SetTexture(const std::string& animationName, int arrayIndex, int materialIndex);
	void UpdateAnimation();
	void Draw(GameParameters* param, Vector3f& position, Vector3f& rotation, Vector3f& scale);
	void InstanceDraw(GameParameters* param, int numDraw, Vector3f& position, Vector3f& rotation, Vector3f& scale);

protected:
	void SetMatrix(GameParameters* param, Vector3f& position, Vector3f& rotation, Vector3f& scale);

public:
	MeshModel* drawModel;
};