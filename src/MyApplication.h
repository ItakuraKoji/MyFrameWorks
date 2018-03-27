#pragma once

#include<GLEW\glew.h>
#include<Eigen\Geometry>
#include<string>
#include<uchar.h>

#include"MeshObject.h"
#include"SpriteObject.h"
#include"ModelDataFactory.h"

#include"MapPolygon.h"

#include"FrameBufferList.h"

#include"MyMathFanctions.h"
#include"GameParameters.h"
#include"Player.h"
#include"StaticObject.h"

#include"FontRenderer.h"

//note
//�V�F�[�_�[���Z�b�g�����ɃJ�������Z�b�g�����Uniform�ϐ��̋������G���[��f���i����ɂ͈�؉e���Ȃ��j
//�g�p�̍ۂ̓V�F�[�_�[���ɃZ�b�g���ׂ�


//���ۂ̃A�v���P�[�V�����̏���������
class MyApplication{
public:
	MyApplication();
	~MyApplication();
	bool Initialize(GLFWwindow* window, int width, int height);
	void Finalize();
	void Run();
	void Draw();


private:
	//��荇�����`��p�X3��
	void DrawPass0();
	void DrawPass1();
	void DrawPass2();

private:
	GameParameters* param;

	MeshObject *square;
	MeshObject *skinModel;
	MeshObject *mapModel;
	SpriteObject* testSprite;
	Player* player;
	StaticObject* mapObj;

	MapPolygon* map;

	FrameBufferList* frameBuffer;
};

