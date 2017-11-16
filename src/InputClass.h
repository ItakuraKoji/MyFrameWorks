#pragma once
#pragma comment(lib, "winmm.lib")
#include<windows.h>
#include<math.h>
#include<iostream>

//�X�e�B�b�N�����ʒu�̗V�ђ萔
#define BUTTON_EPSILON 0.2f

//�Q�[���p�b�h�̃A�i���O�X�e�B�b�N�̒l�̌��_
#define BUTTON_ANALOG_NEUTRAL 32768.f//�Ƃ�XBox�R���g���[���[



//�ڑ������Q�[���p�b�h�̃{�^���Ǝ�
#define PAD_BUTTON_0 1
#define PAD_BUTTON_1 2
#define PAD_BUTTON_2 4
#define PAD_BUTTON_3 8
#define PAD_BUTTON_4 16
#define PAD_BUTTON_5 32
#define PAD_BUTTON_6 64
#define PAD_BUTTON_7 128
#define PAD_BUTTON_8 256
#define PAD_BUTTON_9 512
#define PAD_BUTTON_10 1024
#define PAD_BUTTON_11 2048
#define PAD_BUTTON_12 4096
#define PAD_STICK_X 0
#define PAD_STICK_Y 1
#define PAD_STICK_Z 2
#define PAD_STICK_R 3
#define PAD_STICK_U 4
#define PAD_STICK_V 5

//���z�Q�[���p�b�h�z��p�萔
//ABXY��LR
//�\���L�[
//6���̃A�i���O���i0�`1��float�j
//��L�̃A�i���O������A�������Əc�����Ƃ��Ď��X�e�B�b�N�Q�{�i�E�ƍ���z��j
enum {
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
private:
	JoyStickAxis *x;
	JoyStickAxis *y;
public:
	float GetPosX() { return x->pos; }
	float GetPosY() { return y->pos; }
	void Normalize() {
		if (x == 0 || y == 0) {
			return;
		}
		float len = powf(x->pos, y->pos);
		x->pos /= len;
		y->pos /= len;
	}
};



class InputClass {
public:
	InputClass();
	~InputClass();
	bool Initialize(UINT joyID);
	void Run();
	//�����ꂽ�u��
	bool isPressButton(int buttonID);
	//�����Ă����
	bool isStayButton(int buttonID);
	//�����ꂽ�u��
	bool isReaveButton(int buttonID);

	//�X�e�B�b�N�����擾
	float GetStickState(int axisID);

private:
	float FixStickState(float state);
	void GetInputState();


private:
	//�R���g���[���[ID��JOYINFOEX�\����
	UINT joyID;
	JOYINFOEX joyInfo;

	//���z�R���g���[���[�̃{�^���Ή��Ɠ��͏��̔z��
	JoyButtonState vpadState[10] = {};
	

	//�X�e�B�b�N�̏��
	JoyStickState vpadStickState[2] = {};
	JoyStickAxis vpadStickAxis[6] = {};	//0 �` 1 �̊Ԃɒl�����܂��Ă���
	float stickState[6] = {};		//�l���O����P�Ɏ��܂��Ă��Ȃ�
};