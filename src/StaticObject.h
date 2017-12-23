#pragma once
#include"GameObject.h"
#include"CameraMan.h"
#include"MapPolygon.h"


//あたり判定「だけ」を持つ何もしないオブジェクト
class StaticObject : public GameObject {
public:
	virtual bool Initialize(GameParameters& param);
	virtual void Finalize();
	virtual void Run(GameParameters& param);
	virtual void Draw(GameParameters& param);
	void SetMapCollision(MapPolygon* map);

private:
	MapPolygon* mapCollisions;
};