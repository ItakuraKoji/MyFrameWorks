#pragma once

#include<Eigen\Core>

using namespace Eigen;

enum CharacterState {
	Normal,
	Damege,
	Dead,
};

//�L�����N�^�[�̎��X�e�[�^�X�A�O������̃A�N�Z�X�͋�����\��
struct CharacterParameter {
	CharacterState state;//���
	int HP;//�q�b�g�|�C���g
	int EPLevel;//�d�̓��x��
	int EP;//���ݎg�p�\�ȓd��
};
