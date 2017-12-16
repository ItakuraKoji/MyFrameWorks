#pragma once

#include"ParticleParameter.h"
#include<Eigen\Geometry>
#include<vector>


//�p�[�e�B�N���̏����Ǘ�����A���̃N���X�ɂ͕`��@�\�ɑ������郁�]�b�h�͂Ȃ�
class Particle {
public:
	Particle(ParticleParameter& parameter);
	~Particle();
	bool Initialize(ParticleParameter& parameter);
	void Run();
	void GetParticleMatrix(Matrix4f& mat);
	void GetParticleColor(Vector4f& color);
	bool isAlive();

private:
	ParticleParameter parameter;

};