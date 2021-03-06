#pragma once

#include<string>
#include"AudioData.h"
#include"WavData.h"
#include"OggData.h"

namespace K_Audio {

	//指定されたファイルパスの拡張子を用いて適切にAudioLoaderを生成するファクトリー
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
