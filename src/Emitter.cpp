#include"Emitter.h"


Emitter::Emitter() {

}
Emitter::~Emitter() {
	if (this->test) {
		delete this->test;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &this->particleMatrixBuffer);

	for (auto i : this->particleList) {
		delete i;
	}
}

bool Emitter::Initialize(GameParameters& param) {
	this->numParticle = 0;
	this->numMaxParticle = 60000;
	this->parameter.emitInterval = 0;
	this->parameter.emitQuantity = 50;
	this->parameter.firstLifeLimit = 100;
	this->parameter.firstPosition << 0.0f, 0.0f, 0.0f;
	this->parameter.firstPositionVelosity << 0.0f, -0.1f, 0.0f;
	this->classCount = 0;

	ModelDataFactory factory;

	param.textureList->LoadTexture("particle.tga", "particle.tga");
	this->test = new MeshModel(factory.CreateSquareModel("particle.tga", param));

	this->test->BindVAO();
	//GLSL では Mat4 の入力は4つの Vec4 として扱われる
	glGenBuffers(1, &this->particleMatrixBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->particleMatrixBuffer);
	int location = 2;
	for (int i = 0; i < 4; ++i) {
		glEnableVertexAttribArray(location + i);
		glVertexAttribPointer    (location + i, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (unsigned char*)NULL + 4 * i * sizeof(float));
		glVertexAttribDivisor    (location + i, 1);
	}

	glGenBuffers(1, &this->particleColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->particleColorBuffer);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
	return true;
}

void Emitter::Run() {
	KillParticle();
	EmitParticle();
	UpDateParticle();
}

void Emitter::Draw(GameParameters& param) {
	if (!numParticle) {
		return;
	}

	//すべてのパーティクルの変形行列を取得してシェーダーの入力に設定
	int count = 0;
	Matrix4f *mat = new Matrix4f[this->numParticle];
	Vector4f *color = new Vector4f[this->numParticle];
	for (auto i : this->particleList) {
		i->GetParticleMatrix(mat[count]);
		i->GetParticleColor(color[count]);

		mat[count] = this->projection * this->view * mat[count] * this->world;
		++count;
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->particleMatrixBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->numMaxParticle * sizeof(float) * 16, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->numParticle * sizeof(float) * 16, mat);

	glBindBuffer(GL_ARRAY_BUFFER, this->particleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->numMaxParticle * sizeof(float) * 4, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->numParticle * sizeof(float) * 4, color);

	this->test->InstanceDraw(this->numParticle, param, "static");
	delete mat;
	delete color;
}

void Emitter::SetMatrix(Matrix4f& world, Matrix4f& view, Matrix4f& projection) {
	this->world = world;
	this->view = view;
	this->projection = projection;
}

////////
//private
////

void Emitter::EmitParticle() {
	if (this->classCount < this->parameter.emitInterval) {
		return;
	}
	if (this->numParticle > this->numMaxParticle) {
		return;
	}

	this->classCount = 0;
	for (int i = 0; i < this->parameter.emitQuantity; ++i) {
		ParticleParameter pp;
		pp.lifeLimit        = this->parameter.firstLifeLimit;
		pp.position         = this->parameter.firstPosition;
		pp.position         << ((float)rand() / (RAND_MAX)-0.5f) * 200.0f, ((float)rand() / (RAND_MAX)-0.5f) * 200.0f, ((float)rand() / (RAND_MAX)-0.5f) * 200.0f;
		pp.positionVelosity = this->parameter.firstPositionVelosity;
		pp.positionVelosity << 0.0f, 0.01f + 0.01f * ((float)rand() / (RAND_MAX)), 0.0f;
		pp.firstColor << (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX), 1.0f;
		

		Particle* particle = new Particle(pp);
		this->particleList.push_back(particle);
		++this->numParticle;
	}
}

void Emitter::KillParticle() {
	for (auto it = this->particleList.begin(); it != this->particleList.end();) {
		if (!(*it)->isAlive()) {
			delete (*it);
			std::swap((*it), this->particleList.back());
			this->particleList.pop_back();
			--this->numParticle;
			continue;
		}
		++it;
	}
}

void Emitter::UpDateParticle() {
	for (auto i : this->particleList) {
		i->Run();
	}
	++this->classCount;
}
