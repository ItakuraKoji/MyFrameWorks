#pragma once

#include"SquareModel.h"
#include"ParticleParameter.h"
#include<Eigen\Geometry>
#include<vector>


//パーティクルの情報を管理する、このクラスには描画機能に相当するメゾッドはない
class Particle {
public:
	Particle();
	~Particle();
	bool Initialize(ParticleParameter& parameter);
	void Run();
	void GetParticleMatrix(Matrix4f &mat);
	void GetParticleColor(Vector4f &color);
	bool isAlive();

private:
	ParticleParameter parameter;

};