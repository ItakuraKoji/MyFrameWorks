#pragma once
#include<GLEW\glew.h>
#include<Eigen\Geometry>
#include<bullet\btBulletDynamicsCommon.h>
#include<string>

#include"MeshObject.h"
#include"ModelDataFactory.h"

#include"MapPolygon.h"

#include"ShaderList.h"
#include"LightList.h"


#include"CameraClass.h"
#include"InputClass.h"
#include"Emitter.h"
#include"FrameBuffer.h"

#include"BulletPhysics.h"
#include"GameParameters.h"
#include"MyMathFanctions.h"
#include"Player.h"
#include"StaticObject.h"


//実際のアプリケーションの処理をする
class MyApplication{
public:
	MyApplication();
	~MyApplication();
	bool Initialize(int width, int height);
	void Finalize();
	void Run();
	void Draw();


private:
	//取り合えず描画パス3つ
	void DrawPass0();
	void DrawPass1();
	void DrawPass2();

private:
	GameParameters* param;

	MeshObject *square;
	MeshObject *skinModel;
	MeshObject *mapModel;
	Player* player;
	StaticObject* mapObj;

	//Emitter *model;
	MapPolygon* map;

	Framebuffer* buffer;
};

