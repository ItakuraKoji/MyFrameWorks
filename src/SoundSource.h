#pragma once

#include<OpenAL\al.h>
#include<string>
#include<thread>
#include<mutex>
#include<iostream>


#include"AudioDataFactory.h"


//音源クラス。ループフラグが下りているときはファイル終端に到達した時点でストリーミングを終える
class SoundSource {
public:
	SoundSource(const char* sourceName, const char* filePass, int numBuffer = 32);
	~SoundSource();
	void Play(bool loop);
	void Pause();
	void Stop();

	void SetVolume(float volume);
	void SetPosition(float x, float y, float z);
	void SetVelocity(float x, float y, float z);
	bool IsPlay();

private:
	void StreamingThread();
	void EndStreamingThread();
	void FillBuffer();
	int ReadBuffer(ALuint buffer, int maxReadSize);
	int OggCommentValue(vorbis_comment* comment, const char* key);

private:
	const std::string name;
	AudioData* audio;

	std::thread* streamingThread;
	std::recursive_mutex _mutex;


	ALuint sourceID;
	ALuint *bufferIDs;
	const int numBuffer;

	int loopStart;
	int loopLength;
	ALuint format;
	ALint samplingRate;
	int blockSize;

	float volume;
	float posX, posY, posZ;
	float velocityX, velocityY, velocityZ;

	bool isPlayed;
	bool isLoop;
	bool isEnd;
};