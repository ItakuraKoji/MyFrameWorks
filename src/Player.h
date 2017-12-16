#pragma once
#include"GameObject.h"

class Player : public GameObject {
public:
	virtual bool Initialize(GameParameters& param);
	virtual void Finalize();
	virtual void Run(GameParameters& param);
	virtual void Draw(GameParameters& param);

private:
	btRigidBody* characterCollision;
	float velocity;
	float camerarotH, camerarotV;
};