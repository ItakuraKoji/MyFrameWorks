#pragma once

#include"AudioData.h"
#include<libOggVorbis\vorbis\vorbisfile.h>
#include<string>

namespace K_Audio {

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

}