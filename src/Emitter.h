#pragma once


#include"SquareModel.h"
#include"ParticleParameter.h"
#include"Particle.h"
#include<vector>
#include<random>

//ビルボードのパーティクルの生成制御クラス
class Emitter {
public:
	Emitter();
	~Emitter();
	bool Initialize();
	void Run();
	void Draw();
	void SetMatrix(Matrix4f& world, Matrix4f& view, Matrix4f& projection);
	void SetShader(ShaderClass* shader);

private:
	void EmitParticle();
	void KillParticle();
	void UpDateParticle();


private:
	int classCount;
	SquareModel* billboard;
	GLuint particleMatrixBuffer;
	GLuint particleColorBuffer;

	EmitterParameter parameter;
	std::vector<Particle*> particleList;

	int numParticle;
	int numMaxParticle;
};