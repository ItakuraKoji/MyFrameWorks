#pragma once

#include<OpenAL\al.h>
#include<OpenAL\alc.h>
#include<unordered_map>
#include"SoundSource.h"

//OpenALをラッピングしたクラス
class SoundClass {
public:
	SoundClass();
	~SoundClass();

	bool CreateSource(const char* sourceName, const char* filePass);
	void DeleteSource(const char* sourceName);

	SoundSource* GetSource(const char* sourceName);

private:
	ALCdevice* device;
	ALCcontext* context;

	std::unordered_map<std::string, SoundSource*> source;
};
