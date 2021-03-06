#pragma once

#include<fstream>
#include"AudioData.h"

namespace K_Audio {

	//Waveの読み込みとデータ保持を担当する
	class WavData : public AudioData {
	public:


	public:
		WavData(const char* filePass);
		~WavData();

		void Seek(int offset);
		int Read(char* buffer, int maxSize);


	private:
		bool LoadFile(const char* filePass);
		void PcmSeek(int pcmOffset);

	private:
		std::ifstream waveFile;
		int dataStartOffset;
	};

}