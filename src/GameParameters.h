#pragma once
#include"TextureList.h"
#include"ShaderList.h"
#include"LightList.h"
#include"CameraList.h"
#include"InputClass.h"
#include"BulletPhysics.h"

#include"AudioDataFactory.h"
#include"SoundClass.h"

//描画世界パラメーター
//・いい加減、すべてのオブジェクトに共通なデータを個別に渡すのがアホらしくなってきたのでまとめる
//・この構造体さえ渡せば、必要なデータの共通項にすべてアクセスできる
class GameParameters {
public:
	GameParameters();
	~GameParameters();
	bool Initialize(int screenWidth, int screenHeight);
	void Finalize();
	void Run();

	void UseAmbient(const std::string& name);
	void UseDirectional(const std::string& name);
	void UseShader(const std::string& name);
	void UseCamera(const std::string& name);


	InputClass*    GetInput();
	BulletPhysics* GetPhysics();
	TextureList*   GetTextureList();
	ShaderList*    GetShaderList();
	LightList*     GetLightList();
	CameraList*    GetCameraList();
	SoundClass*    GetAudioList();

private:
	InputClass*    input;
	BulletPhysics* physicsSystem;
	TextureList*   textureList;
	ShaderList*    shaderList;
	LightList*     lightList;
	CameraList*    cameraList;
	SoundClass*    audioPlayer;

public:
	CameraClass*   currentCamera;
	ShaderClass*   currentShader;
	int            screenWidth;
	int            screenHeight;
};