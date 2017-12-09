#pragma once
#include"Model.h"
#include"ModelData.h"
#include"ShaderClass.h"
#include<vector>

//ビルボードなどに使うであろう板ポリゴン、マテリアルは一つのみ
class SquareModel : public Model {
private:
	struct Vertex
	{
		Vector3f pos;
		Vector2f uv;
	};
public:
	SquareModel(std::string textureName, TextureList* list);
	~SquareModel();

	bool Initialize(std::string textureName, TextureList* list);
	void Finalize();
	void SetTexture(std::string textureName);
	void Draw();
	void InstanceDraw(int numInstance);
	void SetShader(ShaderClass* shader);
	void SetMatrix(Matrix4f& world, Matrix4f& view, Matrix4f& projection);

private:
	void InitializeBuffers();
	void InitializeMaterial(std::string textureName);

private:
	VertexData* vertexBuffers;
	MaterialData* materialDatas;
	std::string textureName;
};
