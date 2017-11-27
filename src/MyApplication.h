#pragma once
#include<GLEW\glew.h>
#include<Eigen\Geometry>
#include<bullet\btBulletDynamicsCommon.h>
#include<string>

#include"SkinModel.h"
#include"SquareModel.h"
#include"StaticModel.h"
#include"MapPolygon.h"

#include"ShaderList.h"

#include"CameraClass.h"
#include"Light.h"
#include"InputClass.h"
#include"Emitter.h"
#include"HitPrimitive.h"
#include"FrameBuffer.h"

#include"BulletPhysics.h"

#define Noraml_Rad(deg) (M_PI * (deg) / 180.0f)

//実際のアプリケーションの処理をする
class MyApplication{
public:
	MyApplication();
	~MyApplication();
	bool Initialize(int width, int height);
	void Run();
	void Draw();
	Matrix4f MatrixPerspectiveLH(float screenWidth, float screenHeight, float fieldOfView, float screenNear, float screenFar);


private:
	void bulletInitialize();

private:
	int screenWidth;
	int screenHeight;
	float fieldOfView;
	Matrix4f projectionMat;


	SkinModel *skinModel;
	StaticModel *mapModel;
	Emitter *model;
	ShaderList *shader;
	CameraClass *camera;
	InputClass *input;
	MapPolygon *map;

	DirectionalLight *light;
	Framebuffer* buffer;

	//とりあえず物理エンジン
	BulletPhysics *physics;
	btCollisionObject *characterCollision;

};

