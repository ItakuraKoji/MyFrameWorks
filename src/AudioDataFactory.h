#pragma once

#include<string>
#include"AudioData.h"
#include"WavData.h"
#include"OggData.h"

namespace K_Audio {

	//�w�肳�ꂽ�t�@�C���p�X�̊g���q��p���ēK�؂�AudioLoader�𐶐�����t�@�N�g���[
	class AudioDataFactory {
	public:
		enum AudioType {
			Wave,
			Ogg,
			NonSupport,
		};

	public:
		AudioDataFactory();
		~AudioDataFactory();
		AudioData* Create(const char* filePass);
	private:
		AudioType CheckType(const char* filePass);
	};

}
