#include"SoundClass.h"

SoundClass::SoundClass() {
	this->device = alcOpenDevice(nullptr);
	if (!this->device) {
		return;
	}
	this->context = alcCreateContext(this->device, nullptr);
	if (!this->context) {
		return;
	}
	alcMakeContextCurrent(this->context);
}
SoundClass::~SoundClass() {
	for (auto i : this->source) {
		delete i.second;
	}

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(this->context);
	alcCloseDevice(this->device);
}

void SoundClass::Run() {
	ALenum a = alcGetError(this->device);
	if (a != ALC_NO_ERROR) {
		std::cout << "error" << std::endl;
	}
}


bool SoundClass::CreateSource(const char* sourceName, const char* filePass) {
	//ソース名の重複は許さない
	if (this->source.find(sourceName) != this->source.end()) {
		return false;
	}
	SoundSource* audioSource = new SoundSource(sourceName, filePass);
	if (!audioSource) {
		return false;
	}

	this->source[sourceName] = audioSource;
	return true;
}

//ソースの明示的解放(ほっといてもデストラクタですべて解放される)
void SoundClass::DeleteSource(const char* sourceName) {
	//ソース名の存在チェック
	if (this->source.find(sourceName) == this->source.end()) {
		return;
	}
	delete this->source[sourceName];
	this->source.erase(sourceName);
}

//ソースの獲得。ポインターを返す関数は本当は作りたくないんだがなあ
SoundSource* SoundClass::GetSource(const char* sourceName) {
	//ソース名の存在チェック
	if (this->source.find(sourceName) == this->source.end()) {
		return nullptr;
	}
	return this->source[sourceName];
}
