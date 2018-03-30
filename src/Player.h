#pragma once
#include"GameObject.h"
#include"CameraMan.h"


class Player : public GameObject {
public:
	virtual bool Initialize(GameParameters* param);
	virtual void Finalize();
	virtual void Run(GameParameters* param);
	virtual void Draw(GameParameters* param);

	void SetCameraMan(K_Graphics::CameraClass* camera);

private:

private:
	K_Physics::CollisionData* characterCollision;
	K_Physics::CollisionData* characterFoot;

	float speed;
	float velocity;
	//移動ベクトルの補正に使う専属のカメラマン
	CameraMan* camera;
};