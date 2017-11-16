#pragma once
#include<Eigen\Core>
using namespace Eigen;

//�L�[�t���[���͖�����
//�L�[�t���[���ň������Ԃ́@0.0f�`1.0f�@�̊ԂƂ���

//�G�~�b�^�̃p�����[�^�[�A�p�[�e�B�N���̏����l�₻�̐����ʂȂǂ̒l������
struct EmitterParameter {
	int firstLifeLimit;
	int emitQuantity;
	int emitInterval;

	//particle�̏����l
	//position
	Vector3f firstPosition;
	Vector3f firstUV;
	Vector2f firstScale;
	//velosity
	Vector3f firstPositionVelosity;
	Vector3f firstUVVelosity;
	Vector3f firstScaleVelosity;
	//color(KeyFrame)
	Vector4f firstColor;
	//std::vector<Vector4f> keyFrameColor;
};

//�p�[�e�B�N���̃p�����[�^�[�A�ʒu�AUV���W�A�T�C�Y�⑬�x������̒l�Ȃǂ�����
struct ParticleParameter {
	int lifeLimit;
	//position
	Vector3f position;
	Vector3f uv;
	Vector2f scale;
	//velosity
	Vector3f positionVelosity;
	Vector3f uvVelosity;
	Vector3f scaleVelosity;
	//color(KeyFrame)
	Vector4f firstColor;
	//std::vector<Vector4f> keyFrameColor;
};