#pragma once
#include<GLEW\glew.h>
#include<Eigen\Geometry>
#include<bullet\btBulletDynamicsCommon.h>
#include<string>

#include"MeshModel.h"
#include"ModelDataFactory.h"

#include"MapPolygon.h"

#include"ShaderList.h"
#include"LightList.h"


#include"CameraClass.h"
#include"InputClass.h"
#include"Emitter.h"
#include"FrameBuffer.h"

#include"BulletPhysics.h"
#include"DrawParameters.h"
#include"MyMathFanctions.h"
#include"Player.h"


//���ۂ̃A�v���P�[�V�����̏���������
class MyApplication{
public:
	MyApplication();
	~MyApplication();
	bool Initialize(int width, int height);
	void Finalize();
	void Run();
	void Draw();


private:
	void bulletInitialize();
	//��荇�����`��p�X3��
	void DrawPass0();
	void DrawPass1();
	void DrawPass2();

private:
	GameParameters param;
	Matrix4f projectionMat;

	MeshModel *square;
	MeshModel *skinModel;
	MeshModel *mapModel;
	Player* player;

	Emitter *model;
	CameraClass* camera;
	MapPolygon* map;

	Framebuffer* buffer;

	//�Ƃ肠���������G���W��
	btCollisionObject *characterCollision;

};

