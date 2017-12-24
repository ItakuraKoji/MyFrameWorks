#pragma once
#include"GameObject.h"

//�J�����N���X�ƁA�ΏۂƂȂ�ق���GameObject�ւ̃|�C���^�����J�����}��
//�^�[�Q�b�g�����苗����ۂ��ē���
class CameraMan : public GameObject {
public:
	bool Initialize(GameParameters* param);
	void Finalize();
	void Run(GameParameters* param);
	void Draw(GameParameters* param);

	void SetParameter(GameObject* terget, CameraClass* camera);
	float GetRotationH();
	float GetRotationV();

private:
	GameObject* terget;
	CameraClass* camera;
	float distance;
	float height;
	float camerarotH, camerarotV;

};