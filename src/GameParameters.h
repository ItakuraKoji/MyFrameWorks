#pragma once
#include"TextureList.h"
#include"ShaderList.h"
#include"LightList.h"
#include"CameraList.h"
#include"InputGLFW.h"
#include"BulletPhysics.h"
#include"FontRenderer.h"

#include"SoundClass.h"
#include"EffectClass.h"


//描画世界パラメーター
//・いい加減、すべてのオブジェクトに共通なデータを個別に渡すのがアホらしくなってきたのでまとめる
//・この構造体さえ渡せば、必要なデータの共通項にすべてアクセスできる
//・ただし、これ、循環参照には気をつけてな
class GameParameters {
public:
	GameParameters(GLFWwindow* window, int screenWidth, int screenHeight);
	~GameParameters();
	bool Initialize(GLFWwindow* window, int screenWidth, int screenHeight);
	void Finalize();
	void Run();

	void UseAmbient(const std::string& name);
	void UseDirectional(const std::string& name);
	void UseShader(const std::string& name);
	void UseCamera(const std::string& name);


	InputGLFW*    GetInput();
	BulletPhysics* GetPhysics();
	TextureList*   GetTextureList();
	ShaderList*    GetShaderList();
	LightList*     GetLightList();
	CameraList*    GetCameraList();
	SoundClass*    GetAudioList();
	EffectClass*   GetEffects();
	FontRenderer*  GetFontRenderer();

private:
	InputGLFW *    input;
	BulletPhysics* physicsSystem;
	TextureList*   textureList;
	ShaderList*    shaderList;
	LightList*     lightList;
	CameraList*    cameraList;
	SoundClass*    audioPlayer;
	EffectClass*   effectSystem;
	FontRenderer*  fontRenderer;

public:
	CameraClass*   currentCamera;
	ShaderClass*   currentShader;
	int            screenWidth;
	int            screenHeight;
};