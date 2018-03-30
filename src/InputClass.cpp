#include"InputClass.h"

namespace K_Input {

	////////
	//public
	////
	InputClass::InputClass(UINT joyID) {
		Initialize(joyID);
	}

	InputClass::~InputClass() {

	}

	//������
	bool InputClass::Initialize(UINT joyID) {
		//���z�L�[�ƃQ�[���p�b�h�̑Ή�
		this->vpadState[VPAD_BUTTON_A].button = PAD_BUTTON_0;
		this->vpadState[VPAD_BUTTON_B].button = PAD_BUTTON_1;
		this->vpadState[VPAD_BUTTON_X].button = PAD_BUTTON_2;
		this->vpadState[VPAD_BUTTON_Y].button = PAD_BUTTON_3;
		this->vpadState[VPAD_BUTTON_L].button = PAD_BUTTON_4;
		this->vpadState[VPAD_BUTTON_R].button = PAD_BUTTON_5;

		//���z�L�[�ƃL�[�{�[�h�̑Ή�
		this->vpadState[VPAD_BUTTON_A].keyboard = VK_SPACE;
		this->vpadState[VPAD_BUTTON_B].keyboard = 'Z';
		this->vpadState[VPAD_BUTTON_RIGHT].keyboard = VK_RIGHT;
		this->vpadState[VPAD_BUTTON_LEFT].keyboard = VK_LEFT;
		this->vpadState[VPAD_BUTTON_UP].keyboard = VK_UP;
		this->vpadState[VPAD_BUTTON_DOWN].keyboard = VK_DOWN;

		//���z�X�e�B�b�N�ƃX�e�B�b�N�̑Ή�
		this->vpadStickAxis[VPAD_STICK_LX].axis = PAD_STICK_X;
		this->vpadStickAxis[VPAD_STICK_LY].axis = PAD_STICK_Y;
		this->vpadStickAxis[VPAD_STICK_RX].axis = PAD_STICK_U;
		this->vpadStickAxis[VPAD_STICK_RY].axis = PAD_STICK_R;
		this->vpadStickAxis[VPAD_STICK_Z1].axis = PAD_STICK_Z;

		//���z�X�e�B�b�N�ƃL�[�{�[�h�̑Ή�
		this->vpadStickAxis[VPAD_STICK_LX].plusButton = VK_RIGHT;
		this->vpadStickAxis[VPAD_STICK_LX].minusButton = VK_LEFT;
		this->vpadStickAxis[VPAD_STICK_LY].plusButton = VK_DOWN;
		this->vpadStickAxis[VPAD_STICK_LY].minusButton = VK_UP;
		this->vpadStickAxis[VPAD_STICK_RX].plusButton = 'D';
		this->vpadStickAxis[VPAD_STICK_RX].minusButton = 'A';
		this->vpadStickAxis[VPAD_STICK_RY].plusButton = 'S';
		this->vpadStickAxis[VPAD_STICK_RY].minusButton = 'W';

		//�Q���X�e�B�b�N��o�^
		this->vpadStickState[VPAD_STICK_L].SetAxisX(&this->vpadStickAxis[VPAD_STICK_LX]);
		this->vpadStickState[VPAD_STICK_L].SetAxisY(&this->vpadStickAxis[VPAD_STICK_LY]);
		this->vpadStickState[VPAD_STICK_R].SetAxisX(&this->vpadStickAxis[VPAD_STICK_RX]);
		this->vpadStickState[VPAD_STICK_R].SetAxisY(&this->vpadStickAxis[VPAD_STICK_RY]);

		this->joyID = joyID;

		this->joyInfo.dwSize = sizeof(JOYINFOEX);
		this->joyInfo.dwFlags = JOY_RETURNALL;

		MMRESULT result;
		result = joyGetPosEx(joyID, &this->joyInfo);
		if (result != JOYERR_NOERROR) {
			//return false;
		}
		return true;
	}

	//���݂̉��z�Q�[���p�b�h�̏�Ԃ��擾
	void InputClass::Run() {
		GetInputState();

		//�{�^���ɑΉ�����r�b�g��AND���Z�����Č��ʂ�z��ɂ����߂�
		for (int i = 0; i < 10; ++i) {
			this->vpadState[i].prevPress = this->vpadState[i].press;
			this->vpadState[i].press = (this->vpadState[i].button & this->joyInfo.dwButtons) || (GetAsyncKeyState(this->vpadState[i].keyboard));
		}
		//�X�e�B�b�N�������z�p�b�h�Ɋ��蓖��
		for (int i = 0; i < 6; ++i) {
			int index = this->vpadStickAxis[i].axis;
			float stateData = this->stickState[index];
			if (GetAsyncKeyState(this->vpadStickAxis[i].plusButton)) {
				stateData = BUTTON_ANALOG_NEUTRAL * 2.0f;
			}
			if (GetAsyncKeyState(this->vpadStickAxis[i].minusButton)) {
				stateData = 0;
			}

			// -1 ���� 1 �͈̔͂̃X�e�B�b�N�ʒu���i�[�����
			this->vpadStickAxis[i].pos = FixStickState(stateData);
		}
	}

	//�L�[�̏�Ԃ��擾
	void InputClass::GetInputState() {
		MMRESULT result;
		result = joyGetPosEx(this->joyID, &this->joyInfo);

		//�Q�[���p�b�h���ڑ�����Ă��Ȃ��ꍇ�̓L�[���͂��Ȃ��������̂Ƃ��Ċi�[
		if (result != JOYERR_NOERROR) {
			this->joyInfo.dwButtons = 0;
			this->joyInfo.dwXpos = (DWORD)BUTTON_ANALOG_NEUTRAL;
			this->joyInfo.dwYpos = (DWORD)BUTTON_ANALOG_NEUTRAL;
			this->joyInfo.dwZpos = (DWORD)BUTTON_ANALOG_NEUTRAL;
			this->joyInfo.dwRpos = (DWORD)BUTTON_ANALOG_NEUTRAL;
			this->joyInfo.dwUpos = (DWORD)BUTTON_ANALOG_NEUTRAL;
			this->joyInfo.dwVpos = (DWORD)BUTTON_ANALOG_NEUTRAL;
		}
		//�X�e�B�b�N�����擾�i�ő�6���j
		this->stickState[PAD_STICK_X] = (float)joyInfo.dwXpos;
		this->stickState[PAD_STICK_Y] = (float)joyInfo.dwYpos;
		this->stickState[PAD_STICK_Z] = (float)joyInfo.dwZpos;
		this->stickState[PAD_STICK_R] = (float)joyInfo.dwRpos;
		this->stickState[PAD_STICK_U] = (float)joyInfo.dwUpos;
		this->stickState[PAD_STICK_V] = (float)joyInfo.dwVpos;
	}

	//�X�e�B�b�N�̒l���C��
	float InputClass::FixStickState(float stickPos) {
		float result;
		if (BUTTON_ANALOG_NEUTRAL == 0) {
			return 0.0f;
		}
		//�X�e�B�b�N�̈ʒu���������l��-1.0f����1.0f�͈̔͂ɏC��
		result = stickPos / BUTTON_ANALOG_NEUTRAL;
		result = roundf(result * 100.0f) / 100.0f - 1.0f;
		//�X�e�B�b�N�̒l�̏����ʒu�ɗV�т���������
		if (result < BUTTON_EPSILON && result > -BUTTON_EPSILON) {
			result = 0.0f;
		}
		return result;
	}

	//�������u��
	bool InputClass::isPressButton(ButtonID buttonID) {
		return this->vpadState[buttonID].press && !this->vpadState[buttonID].prevPress;
	}
	//�����Ă����
	bool InputClass::isStayButton(ButtonID buttonID) {
		return (this->vpadState[buttonID].press && this->vpadState[buttonID].prevPress) || isPressButton(buttonID);
	}
	//�����ꂽ�u��
	bool InputClass::isReaveButton(ButtonID buttonID) {
		return !this->vpadState[buttonID].press && this->vpadState[buttonID].prevPress;
	}

	//�X�e�B�b�N���
	float InputClass::GetStickState(ButtonID axisID) {
		return this->vpadStickAxis[axisID].pos;
	}
	//�p�x�ƌX��
	float InputClass::GetStickRotation(ButtonID stickID) {
		return this->vpadStickState[stickID].GetRotation();
	}
	float InputClass::GetStickPower(ButtonID stickID) {
		float power = this->vpadStickState[stickID].GetPower();
		if (power > 1.0f) {
			power = 1.0f;
		}
		return power;
	}
}