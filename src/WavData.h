#pragma once

#include<fstream>
#include"AudioData.h"

//Wave‚Ì“Ç‚İ‚İ‚Æƒf[ƒ^•Û‚ğ’S“–‚·‚é
class WavData : public AudioData{
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