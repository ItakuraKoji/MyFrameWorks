#include"ModelDataFactory.h"

ModelDatas* ModelDataFactory::LoadFBXModel(const std::string& fileName, DrawParameters& param) {
	ModelDatas* data = new ModelDatas;
	FbxModelLoader loader;
	if(!loader.LoadFBX(fileName, param.textureList)) {
		//要素は全部NULL
		return data;
	}

	//アニメーション情報は存在しない場合がある（NULL）
	data->fbxData      = loader.PassFbxData();
	data->vertexBuffer = loader.PassVertexBuffer();
	data->material     = loader.PassMaterialData();
	data->bone         = loader.PassBoneData();
	data->animation    = loader.PassAnimationData();

	return data;
}

ModelDatas* ModelDataFactory::CreateSquareModel(const std::string& textureName, DrawParameters& param) {
	struct Vertex {
		Vector3f pos;
		Vector2f uv;
	};

	ModelDatas* data = new ModelDatas;
	data->vertexBuffer = new VertexData;
	data->material = new MaterialData;

	Vertex vertex[4];
	vertex[0].pos << -0.1f, 0.1f , 0.0f;
	vertex[1].pos << 0.1f , 0.1f , 0.0f;
	vertex[2].pos << 0.1f , -0.1f, 0.0f;
	vertex[3].pos << -0.1f, -0.1f, 0.0f;

	vertex[0].uv << 0.0f, 1.0f;
	vertex[1].uv << 1.0f, 1.0f;
	vertex[2].uv << 1.0f, 0.0f;
	vertex[3].uv << 0.0f , 0.0f;

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//uv
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + 3 * sizeof(float));



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

	glBindVertexArray(0);

	data->vertexBuffer->Add(vertexBuffer);

	//メッシュ配列は一つだけで、マテリアルもその中に一つだけ持っている
	std::vector<Material> material;
	material.resize(1);

	material[0].ambient << 0.0f, 0.0f, 0.0f, 0.0f;
	material[0].diffuse << 1.0f, 1.0f, 1.0f, 1.0f;
	material[0].specular << 0.0f, 0.0f, 0.0f, 0.0f;
	material[0].power = 0.0f;
	material[0].numFace = 6;

	material[0].textureName = textureName;

	data->material->Add(material);
	return data;
}