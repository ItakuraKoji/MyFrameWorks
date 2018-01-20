#pragma once

#include"AudioData.h"
#include<vorbis\vorbisfile.h>
#include<string>

//Ogg‚Ì“Ç‚İ‚İ‚Æƒf[ƒ^•Û‚ğ’S“–‚·‚é
class OggData : public AudioData {
public:
	OggData(const char* filePass);
	~OggData();

	void Seek(int offset);
	int Read(char* buffer, int maxSize);

private:
	bool LoadFile(const char* filePass);
	int OggCommentValue(vorbis_comment* comment, const char* key);

private:
	OggVorbis_File oggFile;
};