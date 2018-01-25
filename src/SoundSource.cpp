#include"SoundSource.h"

SoundSource::SoundSource(const char* sourceName, const char* filePass, int numBuffer) : name(sourceName), numBuffer(numBuffer) {
	this->bufferIDs = new ALuint[numBuffer];
	this->isPlayed = false;
	this->isLoop = false;
	this->isEnd = false;


	alGenSources(1, &this->sourceID);
	alDistanceModel(AL_EXPONENT_DISTANCE);

	//�ǂݍ���
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

	//�o�b�t�@���������O�ǂݍ���
	for (int i = 0; i < this->numBuffer; ++i) {
		alGenBuffers(1, &this->bufferIDs[i]);
		ReadBuffer(this->bufferIDs[i], 4096);
	}


	//�X���b�h�J�n (�X���b�h�Ƀ����o�[�֐����w�肷��ۂ͑�������this�|�C���^�[���w�肷��)
	this->streamingThread = new std::thread(&SoundSource::StreamingThread, this);
}

SoundSource::~SoundSource() {
	EndStreamingThread();
	this->streamingThread->join();

	//�J��
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

	//�o�b�t�@��������
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

//�X�g���[�~���O���čĐ�����X���b�h
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

//�t���O�𗧂ĂăX���b�h���I��点��
void SoundSource::EndStreamingThread() {
	std::lock_guard<std::recursive_mutex> lock(this->_mutex);
	alSourceStop(this->sourceID);
	alSourcei(this->sourceID, AL_BUFFER, AL_NONE);

	this->isEnd = true;
	this->isPlayed = false;
}

//LOOPSTART��LOOPLENGTH�𑖍�
int SoundSource::OggCommentValue(vorbis_comment* comment, const char* key) {
	int loopStart = 0;
	for (int i = 0; i < comment->comments; ++i) {
		std::string str(comment->user_comments[i]);
		std::string keyword(key);

		//keyword��str�Ƃ̕�����v�𔻒�
		if (str.find(keyword) != std::string::npos) {
			int start = str.find("=") + 1;
			int end = str.length();
			//�R�����g�ɕt�����Ă���PCM�̈ʒu���擾
			return std::stoi(str.substr(start, end - start));
		}
	}
	//������Ȃ��Ƃ��� 0 ��Ԃ�
	return 0;
}

//��{4096�o�C�g�A���ۂɃo�b�t�@��ǂފ֐�
void SoundSource::FillBuffer() {
	int size = 0;

	int fillSize = 4096;
	int offset = this->audio->GetPcmOffset();
	int loopEnd = this->loopStart + this->loopLength;

	//4096�o�C�g��胋�[�v�I�[�܂ł̋������Z���Ƃ��͂�������ǂݍ��ݗʂƂ���
	if (fillSize > (loopEnd - offset) * this->blockSize) {
		fillSize = (loopEnd - offset) * this->blockSize;
	}

	//�����ς݃L���[���Ȃ��ꍇ�͂��̂܂܋A��\��
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
			//���[�v���Ȃ��ꍇ�͏I�[�ɒB������X�g���[�~���O���~
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

//buffer�ɍő�maxReadSize�������ǂ�ŃL���[����A���ۂ̓ǂݍ��݃T�C�Y���Ԃ�
int SoundSource::ReadBuffer(ALuint buffer, int maxReadSize) {

	//readBuffer�����܂�܂œǂ݂���
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