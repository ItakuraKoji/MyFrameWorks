#pragma once
#include"TextureList.h"
#include"ShaderList.h"
#include"CameraClass.h"

//描画世界パラメーター
//・いい加減、すべてのオブジェクトに共通な描画データを渡すのがアホらしくなってきたのでまとめる
//・この構造体さえ渡せば、描画の共通項にすべてアクセスできる
struct DrawParameters {
	TextureList* textureList;
	ShaderList*  shaderList;
	CameraClass* mainCamera;
	int          screenWidth;
	int          screenHeight;
	float        screenFov;
};