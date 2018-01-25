#include"SoundSource.h"

SoundSource::SoundSource(const char* sourceName, const char* filePass, int numBuffer) : name(sourceName), numBuffer(numBuffer) {
	this->bufferIDs = new ALuint[numBuffer];
	this->isPlayed = false;
	this->isLoop = false;
	this->isEnd = false;


	alGenSources(1, &this->sourceID);
	alDistanceModel(AL_EXPONENT_DISTANCE);

	//読み込み
	AudioDataFactory factory;
	this->audio = factory.Create(filePass);

	this->loopStart = this->audio->GetLoopStart();
	this->loopLength = this->audio->GetLoopLength();
	this->samplingRate = this->audio->GetSamplingRate();
	if (this->audio->GetFormat() == SoundFormat::Stereo16) {
		this->format = AL_FORMAT_STEREO16;
		this->blockSize = 4;
	}
	else {
		this->format = AL_FORMAT_MONO16;
		this->blockSize = 2;
	}

	//バッファ生成＆事前読み込み
	for (int i = 0; i < this->numBuffer; ++i) {
		alGenBuffers(1, &this->bufferIDs[i]);
		ReadBuffer(this->bufferIDs[i], 4096);
	}


	//スレッド開始 (スレッドにメンバー関数を指定する際は第二引数にthisポインターを指定する)
	this->streamingThread = new std::thread(&SoundSource::StreamingThread, this);
}

SoundSource::~SoundSource() {
	EndStreamingThread();
	this->streamingThread->join();

	//開放
	delete this->streamingThread;
	delete this->audio;

	alDeleteBuffers(this->numBuffer, this->bufferIDs);
	alDeleteSources(1, &this->sourceID);

	delete[] this->bufferIDs;
}


void SoundSource::Play(bool loop) {
	alSourcePlay(this->sourceID);
	std::lock_guard<std::recursive_mutex> lock(this->_mutex);
	this->isPlayed = true;
	this->isLoop = loop;
}

void SoundSource::Pause() {
	std::lock_guard<std::recursive_mutex> lock(this->_mutex);
	alSourceStop(this->sourceID);
	this->isPlayed = false;
}

void SoundSource::Stop() {
	std::lock_guard<std::recursive_mutex> lock(this->_mutex);
	alSourceStop(this->sourceID);
	this->isPlayed = false;

	//バッファを初期化
	this->audio->Seek(0);
	int num;
	ALuint soundBuffer;
	while (alGetSourcei(this->sourceID, AL_BUFFERS_PROCESSED, &num), num > 0) {
		alSourceUnqueueBuffers(this->sourceID, 1, &soundBuffer);
	}
	for (int i = 0; i < this->numBuffer; ++i) {
		ReadBuffer(this->bufferIDs[i], 4096);
	}
}

void SoundSource::SetVolume(float volume) {
	std::lock_guard<std::recursive_mutex> lock(this->_mutex);
	this->volume = volume;
	alSourcef(this->sourceID, AL_MAX_GAIN, this->volume);
}
void SoundSource::SetPosition(float x, float y, float z) {
	std::lock_guard<std::recursive_mutex> lock(this->_mutex);
	this->posX = x;
	this->posY = y;
	this->posZ = z;
}
void SoundSource::SetVelocity(float x, float y, float z) {
	std::lock_guard<std::recursive_mutex> lock(this->_mutex);
	this->velocityX = x;
	this->velocityY = y;
	this->velocityZ = z;
}

bool SoundSource::IsPlay() {
	return this->isPlayed;
}

////////
//private
////

//ストリーミングして再生するスレッド
void SoundSource::StreamingThread() {
	while (!this->isEnd) {
		if (!this->isPlayed) {
			std::this_thread::sleep_for(std::chrono::milliseconds(600));
			continue;
		}

		ALint state;
		alGetSourcei(this->sourceID, AL_SOURCE_STATE, &state);

		if (state != AL_PLAYING) {
			alSourcePlay(this->sourceID);
		}
		else {
			FillBuffer();
		}
	}
}

//フラグを立ててスレッドを終わらせる
void SoundSource::EndStreamingThread() {
	std::lock_guard<std::recursive_mutex> lock(this->_mutex);
	alSourceStop(this->sourceID);
	alSourcei(this->sourceID, AL_BUFFER, AL_NONE);

	this->isEnd = true;
	this->isPlayed = false;
}

//LOOPSTARTとLOOPLENGTHを走査
int SoundSource::OggCommentValue(vorbis_comment* comment, const char* key) {
	int loopStart = 0;
	for (int i = 0; i < comment->comments; ++i) {
		std::string str(comment->user_comments[i]);
		std::string keyword(key);

		//keywordとstrとの部分一致を判定
		if (str.find(keyword) != std::string::npos) {
			int start = str.find("=") + 1;
			int end = str.length();
			//コメントに付属しているPCMの位置を取得
			return std::stoi(str.substr(start, end - start));
		}
	}
	//見つからないときは 0 を返す
	return 0;
}

//基本4096バイト、実際にバッファを読む関数
void SoundSource::FillBuffer() {
	int size = 0;

	int fillSize = 4096;
	int offset = this->audio->GetPcmOffset();
	int loopEnd = this->loopStart + this->loopLength;

	//4096バイトよりループ終端までの距離が短いときはそっちを読み込み量とする
	if (fillSize > (loopEnd - offset) * this->blockSize) {
		fillSize = (loopEnd - offset) * this->blockSize;
	}

	//処理済みキューがない場合はそのまま帰る予定
	int numProcessed = 0;

	while (size < fillSize && this->isPlayed) {
		alGetSourcei(this->sourceID, AL_BUFFERS_PROCESSED, &numProcessed);
		if (numProcessed == 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(600)); 
			continue;
		}

		ALuint soundBuffer;
		alSourceUnqueueBuffers(this->sourceID, 1, &soundBuffer);

		int readSize = ReadBuffer(soundBuffer, fillSize - size);

		offset = this->audio->GetPcmOffset();
		if (readSize == 0 || offset == loopEnd) {
			//ループしない場合は終端に達したらストリーミングを停止
			if (this->isLoop) {
				this->audio->Seek(this->loopStart);
				offset = this->loopStart;
				//std::cout << "Loop!" << std::endl;
			}
			else {
				Stop();
				break;
			}
		}

		size += readSize;
		if (size == fillSize) {
			break;
		}
		alGetSourcei(this->sourceID, AL_BUFFERS_PROCESSED, &numProcessed);
	}
}

//bufferに最大maxReadSize分だけ読んでキューする、実際の読み込みサイズが返る
int SoundSource::ReadBuffer(ALuint buffer, int maxReadSize) {

	//readBufferが埋まるまで読みこむ
	int readSize = 0;
	char readBuffer[4096];
	while (readSize != maxReadSize) {
		char tmp[4096];
		int singleReadSize = this->audio->Read(tmp, maxReadSize - readSize);
		if (singleReadSize <= 0) {
			break;
		}
		for (int i = 0; i < singleReadSize; ++i) {
			readBuffer[i + readSize] = tmp[i];
		}
		readSize += singleReadSize;
	}
	if (readSize == 0) {
		return 0;
	}
	alBufferData(buffer, this->format, readBuffer, readSize, this->samplingRate);
	alSourceQueueBuffers(this->sourceID, 1, &buffer);

	return readSize;
}