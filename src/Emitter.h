#pragma once

#include"DrawParameters.h"
#include"MeshModel.h"
#include"ModelDataFactory.h"
#include"ParticleParameter.h"
#include"Particle.h"
#include<vector>
#include<random>
#include<Eigen\Core>

//ビルボードのパーティクルの生成制御クラス
class Emitter {
public:
	Emitter();
	~Emitter();
	bool Initialize(DrawParameters& param);
	void Run();
	void Draw(DrawParameters& param);
	void SetMatrix(Matrix4f& world, Matrix4f& view, Matrix4f& projection);

private:
	void EmitParticle();
	void KillParticle();
	void UpDateParticle();


private:
	Matrix4f world;
	Matrix4f view;
	Matrix4f projection;

	int classCount;
	MeshModel* test;

	GLuint particleMatrixBuffer;
	GLuint particleColorBuffer;

	EmitterParameter parameter;
	std::vector<Particle*> particleList;

	int numParticle;
	int numMaxParticle;

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};