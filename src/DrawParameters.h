#pragma once
#include"TextureList.h"
#include"ShaderList.h"
#include"LightList.h"
#include"CameraClass.h"
#include"InputClass.h"
#include"BulletPhysics.h"

//描画世界パラメーター
//・いい加減、すべてのオブジェクトに共通なデータを個別に渡すのがアホらしくなってきたのでまとめる
//・この構造体さえ渡せば、必要なデータの共通項にすべてアクセスできる
class GameParameters {
public:
	InputClass*    input;
	BulletPhysics* physicsSystem;
	TextureList*   textureList;
	ShaderList*    shaderList;
	LightList*     lightList;
	CameraClass*   camera;
	ShaderClass*   currentShader;
	int            screenWidth;
	int            screenHeight;
	float          screenFov;
	float          gravity;
};