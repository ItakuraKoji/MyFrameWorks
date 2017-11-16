#pragma once
#include"Model.h"
#include"FbxModelLoader.h"
#include"ModelData.h"
#include"ShaderClass.h"

class SkinModel : public Model{
public:
	SkinModel();
	~SkinModel();

	bool Initialize(const char* fileName);
	void Run();
	void Draw();

private:
	bool LoadFbxModel(const char* fileName);
	void DrawBuffers(int arrayIndex);

private:
	Texture       *boneTexture;
	FbxData       *fbxData;
	VertexData    *vertexBuffers;
	MaterialData  *materialDatas;
	AnimationData *animationDatas;
	BoneData      *boneDatas;
};