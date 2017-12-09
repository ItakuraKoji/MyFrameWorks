#pragma once
#include"Model.h"
#include"FbxModelLoader.h"
#include"ModelData.h"
#include"ShaderClass.h"

class SkinModel : public Model{
public:
	SkinModel(const char* fileName, TextureList* list);
	~SkinModel();

	bool Initialize(const char* fileName, TextureList* list);
	void Finalize();
	void BindVAO();
	void SetAnimation(std::string animationName, bool isLoop, bool isInterporation, bool playOnce);
	void SetSpeed(int speed);
	void Draw();
	void InstanceDraw(int numInstance);

private:
	bool LoadFbxModel(const char* fileName);
	void SetBone(int arrayIndex);
	void DrawBuffers(int arrayIndex);

private:
	Texture*       boneTexture;
	FbxData*       fbxData;
	VertexData*    vertexBuffers;
	MaterialData*  materialDatas;
	AnimationData* animationDatas;
	BoneData*      boneDatas;
};