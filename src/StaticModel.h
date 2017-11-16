#pragma once
#include"Model.h"
#include"FbxModelLoader.h"
#include"ModelData.h"

class StaticModel : public Model {
public:
	StaticModel();
	~StaticModel();

	bool Initialize(const char* fileName);
	void Run();
	void Draw();

private:
	bool LoadFbxModel(const char* fileName);
	void DrawBuffers(int arrayIndex);

private:
	FbxData       *fbxData;
	VertexData    *vertexBuffers;
	MaterialData  *materialDatas;
};