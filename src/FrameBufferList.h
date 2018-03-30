#pragma once

#include<unordered_map>
#include<string>
#include"FrameBuffer.h"
#include"TextureList.h"

namespace K_Graphics {

	//�t���[���o�b�t�@�������Ǘ��N���X�A�e�N�X�`���������̂�TextureList�Ȃǂ̃e�N�X�`���N���X�Ƃ̘A�g�K�{
	class FrameBufferList {
	public:
		FrameBufferList(TextureList* list);
		~FrameBufferList();


		//�e�N�X�`�����X�g�ɐV�K�쐬���˗����A�t���[���o�b�t�@�쐬�����X�g�o�^
		//�[�x�o�b�t�@���g���܂킷�Ƃ��͎g���܂킷�t���[���o�b�t�@�̖��O�������ɓn��
		bool CreateFrameBuffer(const std::string& name, int width, int height);
		bool CreateFrameBuffer(const std::string& name, const std::string& depthBuffer, int width, int height);
		//�r���[�|�[�g��ݒ肵�A�w��F�ŉ�ʂ��N���A����
		void BeginDraw(const std::string& name, float r, float g, float b, bool notDeleteDepthStencil = false);
		//�o�b�N�o�b�t�@�ւ̏������݂��w��
		void BeginDraw(int viewPortWidth, int viewPortHeight, float r, float g, float b, bool notDeleteDepthStencil = false);
		//�o�C���h�������ĕ`��I��
		void EndDraw();

	private:
		void ClearBuffer(int viewPortWidth, int viewPortHeight, float r, float g, float b, bool notDeleteDepthStencil);

	private:
		std::unordered_map<std::string, Framebuffer*> frameBuffers;

		//�Q�Ɨp�A�f�X�g���N�^�ȂǂŎg��
		TextureList* list;
	};
}
