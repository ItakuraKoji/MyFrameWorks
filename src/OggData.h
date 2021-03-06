#pragma once

#include"AudioData.h"
#include<libOggVorbis\vorbis\vorbisfile.h>
#include<string>

namespace K_Audio {

	//Oggの読み込みとデータ保持を担当する
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