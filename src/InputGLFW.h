#pragma once

#include<Windows.h>
#include<GLFW\glfw3.h>
#include<math.h>
#include<iostream>
#include<Eigen\Core>
#include"MyMathFanctions.h"

namespace K_Input {

	//�X�e�B�b�N�����ʒu�̗V�ђ萔
#define BUTTON_EPSILON 0.2f

//�Q�[���p�b�h�̃A�i���O�X�e�B�b�N�̒l�̌��_
#define BUTTON_ANALOG_NEUTRAL 32768.f//�Ƃ�XBox�R���g���[���[

//�ڑ������Q�[���p�b�h�̃{�^���Ǝ�
	enum BUTTON_BITS {
		PAD_BUTTON_0 = 1,
		PAD_BUTTON_1 = 2,
		PAD_BUTTON_2 = 4,
		PAD_BUTTON_3 = 8,
		PAD_BUTTON_4 = 16,
		PAD_BUTTON_5 = 32,
		PAD_BUTTON_6 = 64,
		PAD_BUTTON_7 = 128,
		PAD_BUTTON_8 = 256,
		PAD_BUTTON_9 = 512,
		PAD_BUTTON_10 = 1024,
		PAD_BUTTON_11 = 2048,
		PAD_BUTTON_12 = 4096,
		PAD_STICK_X = 0,
		PAD_STICK_Y = 1,
		PAD_STICK_Z = 2,
		PAD_STICK_R = 3,
		PAD_STICK_U = 4,
		PAD_STICK_V = 5,
	};

	//���z�Q�[���p�b�h�z��p�萔
	//ABXY��LR
	//�\���L�[
	//6���̃A�i���O���i0�`1��float�j
	//��L�̃A�i���O������A�������Əc�����Ƃ��Ď��X�e�B�b�N�Q�{�i�E�ƍ���z��j
	enum ButtonID {
		VPAD_BUTTON_A,
		VPAD_BUTTON_B,
		VPAD_BUTTON_X,
		VPAD_BUTTON_Y,
		VPAD_BUTTON_R,
		VPAD_BUTTON_L,
		VPAD_BUTTON_RIGHT,
		VPAD_BUTTON_LEFT,
		VPAD_BUTTON_UP,
		VPAD_BUTTON_DOWN,

		VPAD_STICK_LX = 0,
		VPAD_STICK_LY,
		VPAD_STICK_RX,
		VPAD_STICK_RY,
		VPAD_STICK_Z1,
		VPAD_STICK_Z2,

		VPAD_STICK_L = 0,
		VPAD_STICK_R,
	};

	//�W���C�p�b�h�̏��
	struct JoyButtonState {
		int button;			//���z�p�b�h�̂��̃{�^���ɑΉ�����Q�[���p�b�h�̃{�^���̃r�b�g
		int keyboard;		//���z�p�b�h�̂��̃{�^���ɂɑΉ�����L�[
		int press;			//���A���̃{�^����������Ă��邩
		int prevPress;		//�O�t���[���A���̃{�^����������Ă�����
	};

	//�A�i���O�X�e�B�b�N�̎�
	struct JoyStickAxis {
		int axis;			//���z�p�b�h�̂��̎��ɑΉ�����Q�[���p�b�h�̎�
		int plusButton;		//���̎��ɂ����鐶�̕����ɑΉ�����L�[�{�[�h�̃L�[
		int minusButton;	//���̎��ɂ����镉�̕����ɑΉ�����L�[�{�[�h�̃L�[
		float pos;			//�A�i���O�X�e�B�b�N�̈ʒu
	};

	//���Əc�̓񎲂����X�e�B�b�N
	struct JoyStickState {
	public:
		//�����Z�b�g
		void SetAxisX(JoyStickAxis* xAxis) {
			this->x = xAxis;
		}
		void SetAxisY(JoyStickAxis* yAxis) {
			this->y = yAxis;
		}

		//�ʒu
		K_Math::Vector2 GetPosition() {
			return K_Math::Vector2(x->pos, y->pos);
		}
		//�p�x
		float GetRotation() {
			return atan2(y->pos, x->pos);
		}
		//�X��
		float GetPower() {
			return sqrtf(powf(x->pos, 2) + powf(y->pos, 2));
		}
	private:
		JoyStickAxis * x;
		JoyStickAxis* y;
	};



	class InputGLFW {
	public:
		InputGLFW(unsigned int joyID, GLFWwindow* handle);
		~InputGLFW();
		bool Initialize(unsigned int joyID, GLFWwindow* handle);
		void Run();
		//�����ꂽ�u��
		bool isPressButton(ButtonID buttonID);
		//�����Ă����
		bool isStayButton(ButtonID buttonID);
		//�����ꂽ�u��
		bool isReaveButton(ButtonID buttonID);

		//�X�e�B�b�N�����擾
		float GetStickState(ButtonID axisID);
		//�X�e�B�b�N�p�x���擾
		float GetStickRotation(ButtonID stickID);
		//�X�e�B�b�N�̌X�����擾
		float GetStickPower(ButtonID stickID);

	private:
		float FixStickState(float state);
		void GetInputState();


	private:
		GLFWwindow * window;
		//�R���g���[���[ID��JOYINFOEX�\����
		unsigned int joyID;

		//���z�R���g���[���[�̃{�^���Ή��Ɠ��͏��̔z��
		JoyButtonState vpadState[10] = {};


		//�X�e�B�b�N�̏��
		JoyStickState vpadStickState[2] = {};
		JoyStickAxis vpadStickAxis[6] = {};	//0 �` 1 �̊Ԃɒl�����܂��Ă���
		float stickState[6] = {};		//�l���O����P�Ɏ��܂��Ă��Ȃ�
	};

}
