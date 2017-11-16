#pragma once
#include"ModelData.h"
#include"ShaderClass.h"
#include<vector>

//ビルボードなどに使うであろう板ポリゴン、マテリアルは一つのみ
class SquareModel{
private:
	struct Vertex
	{
		Vector3f pos;
		Vector2f uv;
	};
public:
	SquareModel();
	~SquareModel();

	bool Initialize(const char* texturefileName);
	void Run();
	void Draw(int numInstance);
	void SetShader(ShaderClass* shader);
	void SetMatrix(Matrix4f& world, Matrix4f& view, Matrix4f& projection);

private:
	void InitializeBuffers();
	void InitializeMaterial(const char* texturefileName);

private:
	ShaderClass *shader;

	Matrix4f world;
	Matrix4f view;
	Matrix4f projection;

	VertexData *vertexBuffers;
	MaterialData *materialDatas;

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
