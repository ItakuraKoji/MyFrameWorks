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
	//移動ベクトルの補正に使う専属のカメラマン
	CameraMan* camera;
};