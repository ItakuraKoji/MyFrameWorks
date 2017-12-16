#include"Particle.h"

Particle::Particle(ParticleParameter& parameter) {
	Initialize(parameter);
}
Particle::~Particle() {

}

bool Particle::Initialize(ParticleParameter& parameter) {
	this->parameter = parameter;
	return true;
}
void Particle::Run() {
	--this->parameter.lifeLimit;
	this->parameter.position += this->parameter.positionVelosity;
	this->parameter.position.x() += 0.01f * sinf(3.6f * (3.14f * (100 - this->parameter.lifeLimit) / 180.0f));

	float t = (this->parameter.lifeLimit / 100.0f - 0.5f) * 2.0f;
	this->parameter.firstColor(3) = -(t * t) + 1.0f;
}

void Particle::GetParticleMatrix(Matrix4f& mat) {
	mat = Matrix4f::Identity();
	mat(0) = 4.0f;
	mat(5) = 4.0f;
	mat(10) = 4.0f;
	mat(12) = this->parameter.position.x();
	mat(13) = this->parameter.position.y();
	mat(14) = this->parameter.position.z();
}
void Particle::GetParticleColor(Vector4f& color) {
	color = this->parameter.firstColor;
}

bool Particle::isAlive() {
	return this->parameter.lifeLimit > 0;
}