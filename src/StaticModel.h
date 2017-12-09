#pragma once
#include"Model.h"
#include"FbxModelLoader.h"
#include"ModelData.h"

class StaticModel : public Model {
public:
	StaticModel(const char* fileName, TextureList* list);
	~StaticModel();

	bool Initialize(const char* fileName, TextureList* list);
	void Finalize();
	void Draw();
	void InstanceDraw(int numInstance);

private:
	bool LoadFbxModel(const char* fileName);
	void DrawBuffers(int arrayIndex);

private:
	VertexData*   vertexBuffers;
	MaterialData* materialDatas;
};