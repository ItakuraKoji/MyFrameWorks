#include"OggData.h"

namespace K_Audio {

	////////
	//public
	////

	OggData::OggData(const char* filePass) {
		if (!LoadFile(filePass)) {
			throw("Audio Load Error");
		}
	}

	OggData::~OggData() {
		ov_clear(&this->oggFile);
	}

	void OggData::Seek(int offset) {
		ov_pcm_seek(&this->oggFile, offset);
		this->pcmOffset = offset;
	}


	////////
	//private
	////

	bool OggData::LoadFile(const char* filePass) {
		int error = ov_fopen(filePass, &this->oggFile);


		if (error != 0) {
			//�G���[�ڍׂ͖�����
			switch (error) {
			case OV_EREAD:       break;
			case OV_ENOTVORBIS:  break;
			case OV_EVERSION:    break;
			case OV_EBADHEADER:  break;
			case OV_EFAULT:      break;
			default:             break;
			}
			return false; // �G���[
		}

		vorbis_comment* tag = ov_comment(&this->oggFile, -1);
		this->loopStart = OggCommentValue(tag, "LOOPSTART");
		this->loopLength = OggCommentValue(tag, "LOOPLENGTH");

		//���[�v��񂪃R�����g�ɂȂ����͏��߂ƍŌ�����[�v�|�C���g��
		if (!this->loopLength) {
			this->loopLength = ov_pcm_total(&this->oggFile, -1);
		}

		vorbis_info* info;
		info = ov_info(&this->oggFile, -1);

		if (info->channels == 1) {
			this->format = SoundFormat::Mono16;
			this->blockSize = 2;
		}
		else {
			this->format = SoundFormat::Stereo16;
			this->blockSize = 4;
		}
		this->samplingRate = info->rate;
		this->pcmOffset = 0;
		return true;
	}

	int OggData::Read(char* buffer, int maxSize) {
		int bitStream;
		//0, 2, 1�̈Ӗ��͂��ꂼ��u���g���G���f�B�A�����g�p�v�uWORD�̃T�C�Y��16bit�v�uPCM�����t�����g�p�v
		int readSize = ov_read(&this->oggFile, buffer, maxSize, 0, 2, 1, &bitStream);
		this->pcmOffset += readSize / this->blockSize;
		return readSize;
	}

	//����̃��[�U�[�R�����g���������R�Â��Ă���PCM�ʒu��Ԃ�
	int OggData::OggCommentValue(vorbis_comment* comment, const char* key) {
		int loopStart = 0;
		for (int i = 0; i < comment->comments; ++i) {
			std::string str(comment->user_comments[i]);
			std::string keyword(key);

			//keyword��str�Ƃ̕�����v�𔻒�
			if (str.find(keyword) != std::string::npos) {
				int start = str.find("=") + 1;
				int end = str.length();
				//�R�����g�ɕt�����Ă���PCM�̈ʒu���擾
				return std::stoi(str.substr(start, end - start));
			}
		}
		//������Ȃ��Ƃ��� 0 ��Ԃ�
		return 0;
	}

}
