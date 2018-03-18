#pragma once

#include<OpenAL\AL\al.h>
#include<OpenAL\AL\alc.h>
#include<unordered_map>
#include"SoundSource.h"

//OpenALをラッピングしたクラス
class SoundClass {
public:
	SoundClass();
	~SoundClass();

	bool CreateSource(const char* sourceName, const char* filePass, SoundSource::LoadMode mode);
	void DeleteSource(const char* sourceName);

	SoundSource* GetSource(const char* sourceName);

private:
	ALCdevice* device;
	ALCcontext* context;

	std::unordered_map<std::string, SoundSource*> source;
};
