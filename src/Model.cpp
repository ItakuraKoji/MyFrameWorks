#include"Model.h"

Model::Model() {

}
Model::~Model() {

}
void Model::SetShader(ShaderClass* shader) {
	this->shader = shader;
}

void Model::SetMatrix(Matrix4f& world, Matrix4f& view, Matrix4f& projection) {
	this->world = world;
	this->view = view;
	this->projection = projection;
}
