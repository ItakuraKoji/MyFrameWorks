#include"SquareModel.h"
////////
//public
////
SquareModel::SquareModel() {

}
SquareModel::~SquareModel() {
	if (this->vertexBuffers) {
		delete this->vertexBuffers;
	}
	if (this->materialDatas) {
		delete this->materialDatas;
	}
}

bool SquareModel::Initialize(const char* texturefileName) {
	this->vertexBuffers = new VertexData;
	this->materialDatas = new MaterialData;

	InitializeBuffers();
	InitializeMaterial(texturefileName);
	return true;
}

void SquareModel::Run() {

}

void SquareModel::Draw(int numInstance) {
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

	this->shader->SetShader();
	this->shader->SetShaderParameter(this->world.data(), this->view.data(), this->projection.data());
	this->shader->SetShaderTexture(0, this->materialDatas->GetTexture(0, 0)->GetTextureID());

	glBindVertexArray(this->vertexBuffers->GetVAO(0));
	glDrawElementsInstanced(GL_TRIANGLES, this->vertexBuffers->GetNumFace(0) * 3, GL_UNSIGNED_INT, 0, numInstance);

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

void SquareModel::SetShader(ShaderClass *shader) {
	this->shader = shader;
}

void SquareModel::SetMatrix(Matrix4f& world, Matrix4f& view, Matrix4f& projection) {
	this->world = world;
	this->view = view;
	this->projection = projection;
}

////////
//private
////
void SquareModel::InitializeBuffers() {
	Vertex vertex[4];
	vertex[0].pos << -0.1f, 0.1f, 0.0f;
	vertex[0].uv << 0.0f, 0.0f;
	vertex[1].pos << 0.1f, 0.1f, 0.0f;
	vertex[1].uv << 1.0f, 0.0f;
	vertex[2].pos << 0.1f, -0.1f, 0.0f;
	vertex[2].uv << 1.0f, 1.0f;
	vertex[3].pos << -0.1f, -0.1f, 0.0f;
	vertex[3].uv << 0.0f, 1.0f;

	unsigned int index[6];
	index[0] = 0;
	index[1] = 3;
	index[2] = 2;
	index[3] = 0;
	index[4] = 2;
	index[5] = 1;

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertex, GL_STATIC_DRAW);

	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer    (0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//uv
	glEnableVertexAttribArray(1);
	glVertexAttribPointer    (1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + 3 * sizeof(float));



	std::vector<GLuint> IBOs;
	IBOs.resize(1);
	glGenBuffers(1, &IBOs[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), index, GL_STATIC_DRAW);

	VertexBuffer vertexBuffer;
	vertexBuffer.VAO = VAO;
	vertexBuffer.VBO = VBO;
	vertexBuffer.IBOs = IBOs;
	vertexBuffer.numFace = 2;
	vertexBuffer.numMaterial = 1;

	//glBindVertexArray(0);

	this->vertexBuffers->Add(vertexBuffer);
}

void SquareModel::InitializeMaterial(const char* texturefileName) {
	//メッシュ配列は一つだけで、マテリアルもその中に一つだけ持っている
	std::vector<Material> material;
	material.resize(1);

	material[0].ambient << 0.0f, 0.0f, 0.0f, 0.0f;
	material[0].diffuse << 1.0f, 1.0f, 1.0f, 1.0f;
	material[0].specular << 0.0f, 0.0f, 0.0f, 0.0f;
	material[0].power = 0.0f;
	material[0].numFace = 6;

	Texture* texture = new Texture;
	texture->Initialize();
	texture->LoadImage(texturefileName);
	material[0].texture = texture;

	this->materialDatas->Add(material);
}