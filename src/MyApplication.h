#pragma once
#include<GLEW\glew.h>
#include<Eigen\Geometry>
#include<bullet\btBulletDynamicsCommon.h>
#include<string>

#include"MeshModel.h"
#include"ModelDataFactory.h"

#include"MapPolygon.h"

#include"ShaderList.h"

#include"CameraClass.h"
#include"Light.h"
#include"InputClass.h"
#include"Emitter.h"
#include"HitPrimitive.h"
#include"FrameBuffer.h"

#include"BulletPhysics.h"
#include"DrawParameters.h"

#define Noraml_Rad(deg) ((float)M_PI * (deg) / 180.0f)

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
	Matrix4f MatrixPerspectiveLH(float screenWidth, float screenHeight, float fieldOfView, float screenNear, float screenFar);
	void bulletInitialize();
	//取り合えず描画パス3つ
	void DrawPass0();
	void DrawPass1();
	void DrawPass2();

private:
	DrawParameters param;
	int screenWidth;
	int screenHeight;
	float fieldOfView;
	Matrix4f projectionMat;

	MeshModel *square;
	MeshModel *skinModel;
	MeshModel *mapModel;

	Emitter *model;
	TextureList* texture;
	ShaderList* shader;
	CameraClass* camera;
	InputClass* input;
	MapPolygon* map;

	DirectionalLight *light;
	Framebuffer* buffer;

	//とりあえず物理エンジン
	BulletPhysics *physics;
	btCollisionObject *characterCollision;

};

