#pragma once

#define __EFFEKSEER_RENDERER_GL_GLEW__

#include<Effekseer\Effekseer.h>
#include<Effekseer\EffekseerRendererGL.h>
#include<Effekseer\EffekseerSoundAL.h>
#include<unordered_map>
#include<string>
#include<gl\GLU.h>

#include"CameraClass.h"

typedef Effekseer::Handle EffectHandle;

class EffectClass {
public:

public:
	EffectClass();
	~EffectClass();

	void SetMatrix(CameraClass* viewCamera);
	void Run();
	void Draw();

	void AddEffectSource(const char* name, const char* filePass);
	void DeleteEffectSource(const char* name);

	//再生
	//エフェクト名、位置を渡す
	//エフェクトを走査するハンドル番号が返る、移動などの操作をしないなら変数で取っておかなくてもいい
	EffectHandle Play(const std::string& name, float posX, float posY, float posZ);
	//明示的に停止、こちらは呼んだ瞬間にすぐ消える
	void Stop(EffectHandle handle);
	//明示的に停止、ただしエフェクトの生成をやめるだけなのでしばらく残る
	void StopRoot(EffectHandle handle);



private:
	Effekseer::Manager* manager            = nullptr;
	EffekseerRenderer::Renderer* renderer  = nullptr;
	EffekseerSound::Sound* sound           = nullptr;
	std::unordered_map<std::string, Effekseer::Effect*> effect;
	CameraClass* viewCamera;
};
