#include"SoundClass.h"

SoundClass::SoundClass() {
	this->device = alcOpenDevice(nullptr);
	if (!this->device) {
		return;
	}
	this->context = alcCreateContext(this->device, nullptr);
	alcMakeContextCurrent(this->context);
	if (!this->context) {
		return;
	}
}
SoundClass::~SoundClass() {
	for (auto i : this->source) {
		delete i.second;
	}

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(this->context);
	alcCloseDevice(this->device);
}

bool SoundClass::CreateSource(const char* sourceName, const char* filePass) {
	//�\�[�X���̏d���͋����Ȃ�
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

//�\�[�X�̖����I���(�ق��Ƃ��Ă��f�X�g���N�^�ł��ׂĉ�������)
void SoundClass::DeleteSource(const char* sourceName) {
	//�\�[�X���̑��݃`�F�b�N
	if (this->source.find(sourceName) == this->source.end()) {
		return;
	}
	delete this->source[sourceName];
	this->source.erase(sourceName);
}

//�\�[�X�̊l���B�|�C���^�[��Ԃ��֐��͖{���͍�肽���Ȃ��񂾂��Ȃ�
SoundSource* SoundClass::GetSource(const char* sourceName) {
	//�\�[�X���̑��݃`�F�b�N
	if (this->source.find(sourceName) == this->source.end()) {
		return nullptr;
	}
	return this->source[sourceName];
}
