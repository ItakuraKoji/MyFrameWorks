#include"FontRenderer.h"

namespace K_Graphics {

	////////
	//public
	////

	FontRenderer::FontRenderer() {
		//���P�[����ݒ肵�A�t�H���g�����N���X��������
		try {
			setlocale(LC_ALL, "japanese");
			this->sprite = new K_Graphics::SpriteObject(nullptr);
			this->cullentFont = nullptr;
		}
		catch (std::string& errorText) {
			throw(errorText);
		}
	}
	FontRenderer::~FontRenderer() {
		for (int i = 0; i < this->drawData2D.size(); ++i) {
			delete[] this->drawData2D[i].buffer;
		}
		for (auto i : this->generators) {
			if (i.second != nullptr)
				delete i.second;
		}
		if (this->sprite != nullptr) {
			delete this->sprite;
			this->sprite = nullptr;
		}
	}

	bool FontRenderer::LoadFont(const char* fontName, const char* filePass) {
		//���ɂ��閼�O�ł͓o�^�ł��Ȃ�
		if (this->generators.find(fontName) != this->generators.end()) {
			return false;
		}
		//�ǂݍ��݂Ɏ��s���Ă����̊֐��ł͗�O�𓊂��Ȃ�
		try {
			FontGenerator* font = new FontGenerator(filePass);
			this->generators[fontName] = font;
		}
		catch (std::string& errorText) {
			printf("%s\n", errorText.data());
			return false;
		}
		SetCullentFont(fontName);
		return true;
	}
	bool FontRenderer::SetCullentFont(const char* fontName) {
		if (this->generators.find(fontName) == this->generators.end()) {
			return false;
		}
		this->cullentFont = this->generators[fontName];
		return true;
	}

	void FontRenderer::Draw(CameraClass* camera, ShaderClass* shader) {
		if (this->cullentFont == nullptr) {
			for (int i = 0; i < this->drawData2D.size(); ++i) {
				delete this->drawData2D[i].buffer;
			}
			this->drawData2D.clear();
			return;
		}

		//2D
		for (int i = 0; i < this->drawData2D.size(); ++i) {
			//�T�C�Y�ɍ������e�N�X�`����I��
			Texture* texture = this->cullentFont->GetFontAtlas(this->drawData2D[i].fontSize);
			this->sprite->SetTexture(texture);

			//�ʒu�����炵�Ȃ���ꕶ���Â`��
			int position = 0;
			if (this->drawData2D[i].buffer == nullptr) {
				continue;
			}
			for (int j = 0; this->drawData2D[i].buffer[j] != L'\0'; ++j) {
				FontGenerator::FontData data;
				this->cullentFont->GetFontData(data, this->drawData2D[i].buffer[j], this->drawData2D[i].fontSize);
				this->sprite->Draw2D(camera, shader, data.locationUV, K_Math::Box2D(this->drawData2D[i].posX + position, this->drawData2D[i].posY + data.fixPositionY, data.locationUV.w, data.locationUV.h), 0.0f);
				position += data.advance;
			}
			delete[] this->drawData2D[i].buffer;
		}
		this->drawData2D.clear();

		//3D
		//�J�������牓�����Ƀ\�[�g
		std::multimap<float, FontDrawData> ordered;
		for (int i = 0; i < this->drawData3D.size(); ++i) {
			//�J�������W�n��Z���W�Ƀ}�C�i�X������
			float distance = -camera->GetAxisZ().dot(K_Math::Vector3(this->drawData3D[i].posX, this->drawData3D[i].posY, this->drawData3D[i].posZ));
			//���������̏ꍇ�́A��ɌĂ񂾕`�悪�D��
			ordered.insert(std::make_pair(distance, this->drawData3D[i]));
		}
		//�\�[�g�������Ԃŕ`��
		for (auto i : ordered) {
			DrawBuffers3D(i.second, camera, shader);
		}
		this->drawData3D.clear();
	}

	void FontRenderer::DrawString2D(const std::string& text, int size, int posX, int posY) {
		if (this->cullentFont == nullptr) {
			return;
		}

		//���C�h������ɕϊ�
		wchar_t* buffer = new wchar_t[text.size() + 1];
		if (!CreateFontBuffer(buffer, text.data(), text.size() + 1)) {
			delete[] buffer;
			return;
		}
		//�K�v�Ȃ�V�K�e�N�X�`���쐬
		for (int i = 0; buffer[i] != L'\0'; ++i) {
			this->cullentFont->SetFont(buffer[i], size);
		}

		//posZ��2D�ł͗��p���Ȃ�
		FontDrawData drawData;
		drawData.buffer = buffer;
		drawData.fontSize = size;
		drawData.posX = posX;
		drawData.posY = posY;

		this->drawData2D.push_back(drawData);
	}

	void FontRenderer::DrawString3D(const std::string& text, int size, float posX, float posY, float posZ) {
		if (this->cullentFont == nullptr) {
			return;
		}

		//���C�h������ɕϊ�
		wchar_t* buffer = new wchar_t[text.size() + 1];
		if (!CreateFontBuffer(buffer, text.data(), text.size() + 1)) {
			delete[] buffer;
			return;
		}
		//�K�v�Ȃ�V�K�e�N�X�`���쐬
		for (int i = 0; buffer[i] != L'\0'; ++i) {
			this->cullentFont->SetFont(buffer[i], size);
		}
		FontDrawData drawData;
		drawData.buffer = buffer;
		drawData.fontSize = size;
		drawData.posX = posX;
		drawData.posY = posY;
		drawData.posZ = posZ;

		this->drawData3D.push_back(drawData);
	}


	void FontRenderer::ClearFontTexture(int size) {
		this->cullentFont->ClearFontAtlas(size);
	}

	void FontRenderer::ClearAllFontTexture() {
		this->cullentFont->ClearAllFontAtlas();
	}

	////////
	//private
	////

	bool FontRenderer::CreateFontBuffer(wchar_t* result, const char* src, int numChar) {
		size_t bufferSize;
		mbstowcs_s(&bufferSize, result, numChar, src, numChar);
		//�s���ȃ��C�h�����ɂ��ϊ��G���[
		if (bufferSize == -1) {
			//�Ȃ񂩃G���[����������
			return false;
		}
		return true;
	}

	void FontRenderer::DrawBuffers3D(FontDrawData& drawData, CameraClass* camera, ShaderClass* shader) {
		//�T�C�Y�ɍ������e�N�X�`����I��
		Texture* texture = this->cullentFont->GetFontAtlas(drawData.fontSize);
		this->sprite->SetTexture(texture);

		//�ʒu�����炵�Ȃ���ꕶ���Â`��
		int stringSize = 0;
		K_Math::Vector3 position(0.0f, 0.0f, 0.0f);
		K_Math::Vector3 advanceDir = camera->GetAxisX();
		if (drawData.buffer == nullptr) {
			return;
		}
		//���͑S�̂̉��̒������擾
		for (int j = 0; drawData.buffer[j] != L'\0'; ++j) {
			FontGenerator::FontData data;
			this->cullentFont->GetFontData(data, drawData.buffer[j], drawData.fontSize);
			stringSize += data.advance;
		}

		//���̂܂ܕ`�悷��ƃf�J������̂ŕ␳��������
		float size = 0.03f;
		//���ۂ̕`��
		for (int j = 0; drawData.buffer[j] != L'\0'; ++j) {
			FontGenerator::FontData data;
			this->cullentFont->GetFontData(data, drawData.buffer[j], drawData.fontSize);
			K_Math::Vector3 pos3D;
			pos3D.x() = drawData.posX;
			pos3D.y() = drawData.posY - data.fixPositionY * size;
			pos3D.z() = drawData.posZ;
			//�����S�̂̔����������ɂ��炵�ĕ`�悷�邱�ƂŁA�R���g���[���|�C���g�𒆐S��
			pos3D += position - (advanceDir * stringSize / 2.0f * size);

			this->sprite->Draw3D(camera, shader, data.locationUV, pos3D, K_Math::Vector3(0.0f, 0.0f, 0.0f), K_Math::Vector3(size, size, size));
			position += advanceDir * data.advance * size;
		}
		//�g���I����������
		delete[] drawData.buffer;
	}
}

