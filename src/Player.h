#pragma once
#include"GameObject.h"
#include"CameraMan.h"


class Player : public GameObject {
public:
	virtual bool Initialize(GameParameters* param);
	virtual void Finalize();
	virtual void Run(GameParameters* param);
	virtual void Draw(GameParameters* param);

	void SetCameraMan(CameraClass* camera);

private:

private:
	CollisionData* characterCollision;
	CollisionData* characterFoot;

	float speed;
	float velocity;
	//�ړ��x�N�g���̕␳�Ɏg���ꑮ�̃J�����}��
	CameraMan* camera;
};