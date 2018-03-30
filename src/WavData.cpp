#include"WavData.h"

namespace K_Audio {
	////////
	//public
	////

	WavData::WavData(const char* filePass) {
		if (!LoadFile(filePass)) {
			throw("wavData Initialize Failed : " + std::string(filePass));
		}
	}

	WavData::~WavData() {
		this->waveFile.close();
	}

	void WavData::Seek(int offset) {
		//�t�@�C���̐擪����PCM�܂ł̈ʒu�܂ňړ����������Ƃ� (offset * blockSize) �o�C�g���ړ�
		PcmSeek(offset);
		this->pcmOffset = offset;

		//�I�[�𒴂�����I�[�ʒu�ɃV�[�N
		if (this->loopLength < this->pcmOffset) {
			this->pcmOffset = this->loopLength;
			PcmSeek(this->loopLength);
		}
	}

	//maxSize���ǂ�
	int WavData::Read(char* buffer, int maxSize) {
		//�ǂݍ��݃T�C�Y��PCM�̈���T�C�Y�Ŋ���؂�Ȃ��Ȃ玸�s
		if (maxSize % this->blockSize != 0) {
			return 0;
		}
		int readSize = maxSize;
		//�I�[�𒴂��Ȃ��悤�ɃT�C�Y����
		if (this->loopStart + this->loopLength < this->pcmOffset + readSize / this->blockSize) {
			readSize = (this->loopLength - this->pcmOffset) * this->blockSize;
		}

		if (readSize == 0) {
			return 0;
		}

		int prevOffset = this->pcmOffset;
		this->waveFile.read(buffer, readSize);
		this->pcmOffset += readSize / this->blockSize;

		//���ۂɓǂݍ��񂾃T�C�Y�����߂ĕԂ�
		return (this->pcmOffset - prevOffset) * this->blockSize;
	}


	////////
	//private
	////

	//�ǂݍ���
	bool WavData::LoadFile(const char* filePass) {
		//�e�`�����N�̐擪�ɂ��̏�񂪕K������
		struct WaveChunk {
			char id[4];
			int size;
		};
		//fmt�`�����N�p�̍\����
		struct WaveStruct {
			unsigned short formatId;           //�t�H�[�}�b�gID
			unsigned short numChannel;     //�`�����l���� monaural=1 , stereo=2
			unsigned long  samplingRate;     //�P�b�Ԃ̃T���v�����C�T���v�����O���[�g(Hz)
			unsigned long  bytesPerSec;       //�P�b�Ԃ̃f�[�^�T�C�Y
			unsigned short blockSize;          //�P�u���b�N�̃T�C�Y�D8bit:nomaural=1byte , 16bit:stereo=4byte
			unsigned short bitsPerSample;     //�P�T���v���̃r�b�g�� 8bit or 16bit
		};


		this->waveFile.open(filePass, std::ifstream::binary);
		if (!this->waveFile) {
			return false;
		}
		//RIFF�`�����N�̐擪12�o�C�g
		//id = 4bite : size = 4bite;
		WaveChunk chunk;
		WaveStruct fmtChunk;
		char format[4];

		this->waveFile.read((char*)&chunk, 8);
		this->waveFile.read(format, 4);

		//WAVE�t�H�[�}�b�g�ȊO�͎��s
		if (strncmp(chunk.id, "RIFF", 4) != 0 || strncmp(format, "WAVE", 4)) {
			return false;
		}

		//�e�`�����N��ǂ�
		int count = 0;
		int byteOffset = 12;
		int dataSize = 0;
		int fileSize = chunk.size;
		while (byteOffset < fileSize) {
			this->waveFile.read((char*)&chunk, sizeof(WaveChunk));
			if (strncmp(chunk.id, "fmt ", 4) == 0) {
				//fmt�`�����N
				this->waveFile.read((char*)&fmtChunk, chunk.size);
				if (fmtChunk.formatId != 1) {
					return false;
				}
				byteOffset += chunk.size + sizeof(WaveChunk);
				++count;
			}
			else if (strncmp(chunk.id, "data ", 4) == 0) {
				//data�`�����N
				//�ʒu��ۑ����Ē����ǂݍ��ݗp�̃f�[�^�ɗ��p
				this->dataStartOffset = byteOffset + sizeof(WaveChunk);
				this->waveFile.seekg(chunk.size, std::ios_base::cur);
				byteOffset += chunk.size + sizeof(WaveChunk);
				dataSize = chunk.size;
				++count;
			}
			else {
				//����ȊO�̃`�����N�̓X�L�b�v
				this->waveFile.seekg(chunk.size, std::ios_base::cur);
				byteOffset += chunk.size + sizeof(WaveChunk);
			}
		}

		//fmt,data�̃`�����N���킹�ĂQ�łȂ����͎��s
		if (count != 2) {
			return false;
		}

		this->samplingRate = fmtChunk.samplingRate;
		if (fmtChunk.numChannel == 1) {
			//���m����
			if (fmtChunk.bitsPerSample == 8) {
				this->format = SoundFormat::Mono8;
			};
			if (fmtChunk.bitsPerSample == 16) {
				this->format = SoundFormat::Mono16;
			};
		}
		else {
			//�X�e���I
			if (fmtChunk.bitsPerSample == 8) {
				this->format = SoundFormat::Stereo8;
			};
			if (fmtChunk.bitsPerSample == 16) {
				this->format = SoundFormat::Stereo16;
			};
		}

		this->blockSize = fmtChunk.blockSize;

		//ogg�ƈ���Ă������̓��[�v�|�C���g���ŏ��ƍŌ�ŌŒ�
		this->loopStart = 0;
		this->loopLength = this->loopStart + dataSize / this->blockSize;

		Seek(0);
		return true;
	}

	//Wave�t�@�C����PCM�f�[�^�̃V�[�N
	void WavData::PcmSeek(int pcmOffset) {
		this->waveFile.seekg(this->dataStartOffset + pcmOffset * this->blockSize, std::ios_base::beg);
	}
}
