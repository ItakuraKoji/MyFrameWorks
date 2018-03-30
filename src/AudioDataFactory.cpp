#include"AudioDataFactory.h"

namespace K_Audio {
	////////
	//public
	////
	AudioDataFactory::AudioDataFactory() {

	}
	AudioDataFactory::~AudioDataFactory() {

	}
	AudioData* AudioDataFactory::Create(const char* filePass) {
		AudioData* data;

		switch (CheckType(filePass))
		{
		case AudioType::Wave:
			data = new WavData(filePass);
			break;
		case AudioType::Ogg:
			data = new OggData(filePass);
			break;

		case AudioType::NonSupport:
		default:
			data = nullptr;
		}

		return data;
	}

	////////
	//private
	////

	//�g���q�����ނ𔻕�
	AudioDataFactory::AudioType AudioDataFactory::CheckType(const char* filePass) {
		//�g���q�͏��߂́u.�v����I�[�܂ł̕�����Ƃ���
		int i = 0;
		char ext[16] = {};
		while (filePass[i] != '.') {
			++i;
		}
		//�h�b�g�͔�΂�
		++i;

		//�I�[�܂ŃR�s�[
		int extCount = 0;
		while (filePass[i] != 0 && extCount < 15) {
			ext[extCount] = filePass[i];
			++extCount;
			++i;
		}
		ext[extCount] = 0;

		AudioType type = AudioType::NonSupport;
		if (strcmp(ext, "ogg") == 0) {
			type = AudioType::Ogg;
		}
		if (strcmp(ext, "wav") == 0) {
			type = AudioType::Wave;
		}

		return type;
	}

}

