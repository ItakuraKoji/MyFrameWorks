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
//シェーダーをセットせずにカメラをセットするとUniform変数の挙動がエラーを吐く（動作には一切影響ない）
//使用の際はシェーダーを先にセットすべし


//実際のアプリケーションの処理をする
class MyApplication{
public:
	MyApplication();
	~MyApplication();
	bool Initialize(GLFWwindow* window, int width, int height);
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
	SpriteObject* testSprite;
	Player* player;
	StaticObject* mapObj;

	MapPolygon* map;

	FrameBufferList* frameBuffer;
};

