#include"ImageLoader.h"

namespace K_Loader {

	////////
	//public
	////

	//TGA�t�@�C����ǂݍ���
	bool ImageLoader::LoadTGAImage(const std::string& fileName, GLuint TextureID, unsigned int &returnWidth, unsigned int &returnHeight) {
		std::ifstream file;

		TGAHeader header;
		unsigned int numColor;
		GLenum glColorFormat, tgaColorFormat;


		file.open(fileName, std::ios::binary | std::ios::in);
		if (file.fail()) {
			return false;
		}
		file.read((char*)&header, sizeof(TGAHeader));


		int idSize = (int)header.idSize;
		int type = (int)header.type;
		int width = (int)header.width;
		int height = (int)header.height;
		int bpp = (int)header.bpp;
		int descriptor = (int)header.descriptor;

		//���܂̂Ƃ����RGB�`����TGA�C���[�W����������
		if (type != 2 && type != 10) {
			file.close();
			return false;
		}

		//�[�x��񂩂�F�̃t�H�[�}�b�g������
		if (bpp == 32) {
			numColor = 4;
			glColorFormat = GL_RGBA;
			tgaColorFormat = GL_BGRA;
		}
		else if (bpp == 24) {
			numColor = 3;
			glColorFormat = GL_RGBA;
			tgaColorFormat = GL_BGR;
		}
		else {
			file.close();
			return false;
		}

		//�z��T�C�Y�Ɣz��̃f�[�^��ݒ�
		int imagesize = width * height * numColor;
		char* tgaPreImage = new char[imagesize];
		char* tgaImage = new char[imagesize];

		file.seekg(idSize, std::ios::cur);
		file.read(tgaPreImage, imagesize);

		//RLE���kRGB�f�[�^
		if (type == 10) {
			//�ǂݍ��񂾃f�[�^�����ƂɃf�R�[�h�̃f�[�^��tgaPreImage�̃o�b�t�@��u��������i���̃f�[�^��delete�j
			char* temp = tgaPreImage;
			tgaPreImage = new char[imagesize];
			DecodeRLEImage(tgaPreImage, temp, width, height, numColor);
			delete[] temp;
		}

		SetTgaData(tgaImage, tgaPreImage, width, height, numColor, descriptor & 0x10, descriptor & 0x20);

		file.close();

		returnWidth = width;
		returnHeight = height;

		glBindTexture(GL_TEXTURE_2D, TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, glColorFormat, width, height, 0, tgaColorFormat, GL_UNSIGNED_BYTE, tgaImage);

		//�͈͊O�̕\��
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//�g��k�����̕⊮
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//�~�b�v�}�b�v���쐬
		glGenerateMipmap(GL_TEXTURE_2D);

		delete[] tgaPreImage;
		delete[] tgaImage;
		tgaImage = 0;

		return true;
	}

	////////
	//private
	////

	void ImageLoader::SetTgaData(char* data, char* src, int width, int height, int numColor, bool xReverse, bool yReverse) {
		int count = 0;
		//X:�E���獶�@Y:�������@�𐳂̕����Ƃ���
		//XY�ɔ��]�Ȃ�
		if (!(xReverse) && !(yReverse)) {
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					for (int i = 0; i < numColor; ++i) {
						data[count] = src[(y * width + x) * numColor + i];
						++count;
					}
				}
			}
		}
		//X�̂ݔ��]
		if ((xReverse) && !(yReverse)) {
			for (int y = 0; y < height; ++y) {
				for (int x = width - 1; x >= 0; --x) {
					for (int i = 0; i < numColor; ++i) {
						data[count] = src[(y * width + x) * numColor + i];
						++count;
					}
				}
			}
		}
		//Y�̂ݔ��]
		if (!(xReverse) && (yReverse)) {
			for (int y = height - 1; y >= 0; --y) {
				for (int x = 0; x < width; ++x) {
					for (int i = 0; i < numColor; ++i) {
						data[count] = src[(y * width + x) * numColor + i];
						++count;
					}
				}
			}
		}
		//�������]
		if ((xReverse) && (yReverse)) {
			for (int y = height - 1; y >= 0; --y) {
				for (int x = width - 1; x >= 0; --x) {
					for (int i = 0; i < numColor; ++i) {
						data[count] = src[(y * width + x) * numColor + i];
						++count;
					}
				}
			}
		}
	}

	//RLE���k���𓀂���
	void ImageLoader::DecodeRLEImage(char* data, char* src, int width, int height, int numColor) {
		int imageSize = width * height * numColor;
		char* colorData = new char[numColor];
		int count = 0;
		int srcOffset = 0;

		while (count < imageSize) {
			//���f�[�^��RLE���k�f�[�^���𔻕�
			int packet = src[srcOffset];
			++srcOffset;

			if (packet & 0x80) {
				//RLE
				for (int i = 0; i < numColor; ++i) {
					colorData[i] = src[srcOffset];
					++srcOffset;
				}

				int loopCount = (packet & 0x7F) + 1;
				for (int i = 0; i < loopCount; ++i) {
					for (int j = 0; j < numColor; ++j) {
						data[count] = colorData[j];
						++count;
					}
				}
			}
			else {
				//��
				int loopCount = (packet + 1) * numColor;
				for (int i = 0; i < loopCount; ++i) {
					data[count] = src[srcOffset];
					++count;
					++srcOffset;
				}
			}
		}


		delete[] colorData;
	}
}
