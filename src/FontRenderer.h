#pragma once
#include"FontGenerator.h"
#include"MeshModel.h"
#include"MyMathFanctions.h"
#include<map>

namespace K_Graphics {

	//�t�H���g�𐶐����A�`�悷��N���X(���C�h������)
	class FontRenderer {
	private:
		struct FontDrawData {
			wchar_t* buffer;//�o�͂��镶����
			int fontSize;//�T�C�Y
			float posX;//X���W
			float posY;//Y���W
			float posZ;//Z���W�i3D�`��ɗ��p�j
		};

	public:
		FontRenderer();
		~FontRenderer();
		//���O�ɌĂ΂ꂽ�`�施�߂������ōs��
		void Draw(CameraClass* camera, ShaderClass* shader);

		//FontGenerator��������
		bool LoadFont(const char* fontName, const char* filePass);
		//�g�p����t�H���g��ݒ�A���s�����false��Ԃ��A�Z�b�g����Ȃ�
		bool SetCullentFont(const char* fontName);

		//�w����W����e�L�X�g��`��i�ʒu�̓X�N���[���̍��W�j
		void DrawString2D(const std::string& text, int fontSize, int posX, int posY);
		//�w����W����e�L�X�g��`��i�ʒu�͋�Ԃ̍��W�j
		void DrawString3D(const std::string& text, int fontSize, float posX, float posY, float posZ);

		//�t�H���g�`��ɗ��p���Ă���e�N�X�`�����폜����
		void ClearFontTexture(int size);
		//�t�H���g�`��ɗ��p���Ă���e�N�X�`�������ׂč폜����
		void ClearAllFontTexture();

	private:
		bool CreateFontBuffer(wchar_t* result, const char* src, int numChar);
		void DrawBuffers3D(FontDrawData& data, CameraClass* camera, ShaderClass* shader);

	private:
		FontGenerator * cullentFont;
		std::unordered_map<std::string, FontGenerator*> generators;
		K_Graphics::SpriteObject* sprite;

		//�Ă΂ꂽ�`�施�߂ɕK�v�ȏ����i�[���Ă����āADraw()�̎��Ɏg�p
		std::vector<FontDrawData> drawData2D;
		std::vector<FontDrawData> drawData3D;
	};

}
