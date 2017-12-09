#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include<FBX\fbxsdk.h>
#include<GLEW\glew.h>
#include<Eigen\Core>
#include<Eigen\Geometry>
#include"TextureList.h"

using namespace Eigen;
struct Material {
	std::string materialName;
	std::string textureName;
	int numFace;
	Vector4f diffuse;
	Vector4f ambient;
	Vector4f specular;
	float power;
};
struct VertexBuffer {
	GLuint VAO;
	GLuint VBO;
	std::vector<GLuint> IBOs;
	int numMaterial;
	int numFace;
};
struct Bone {
	FbxCluster* cluster;
	Matrix4f bindMat;
	Matrix4f currentMat;
	Matrix4f interPolationMat;
};
struct AnimType {
	std::string animName;
	int animID;
	int startTime;
	int endTime;
};

//外部から読み込まれた、あるいは作られたマテリアルの情報を保持する
//このクラスが保持しているテクスチャの解放責任を持つ
class MaterialData {
public:
	MaterialData();
	~MaterialData();
	bool Initialize();

	void Add(std::vector<Material> material);
	void SetTextureName(const std::string& textureName, int arrayIndex, int materialIndex);
	Material& GetMaterial(int arrayIndex, int materialIndex);

	int GetNumMaterial(int arrayIndex);
	int GetNumFace(int arrayIndex, int materialIndex);

private:
	std::vector<std::vector<Material>> materialArray;
};

//外部から読み込まれた、あるいは作られた頂点バッファの類を保持する
//このクラスは、VAO、VBO、IBO、の３つについて解放責任を持つ
class VertexData {
public:
	VertexData();
	~VertexData();
	bool Initialize();

	void Add(VertexBuffer& buffer);
	int GetNumBuffer();

	GLuint GetVAO(int arrayIndex);
	GLuint GetVBO(int arrayIndex);
	GLuint GetIBO(int arrayIndex, int materialIndex);
	int GetNumFace(int arrayIndex);

private:
	std::vector<VertexBuffer> bufferArray;
};

//3Dアニメーション情報
class AnimationData {
public:
	AnimationData();
	~AnimationData();
	bool Initialize();
	void UpdateAnimation();
	void SetSpeed(int speed);
	void Add(AnimType& animData);
	void SetAnimation(const std::string& animName, FbxScene* fbxScene, bool playOnce, bool loop);
	int GetCurrentAnimTime();

private:
	std::unordered_map<std::string, AnimType> animList;
	int speed;
	int currentAnimID;
	int currentAnimTime;
	int maxAnimTime;
	bool isLoop;
	bool isInterpolation;
};

//ボーン情報とFBXCluster
class BoneData {
public:
	BoneData();
	~BoneData();
	bool Initialize();
	void Add(std::vector<Bone>& boneData);
	void SetClurrentBoneData(int arrayIndex, int time);
	void SetMatrixTextureData(int arrayIndex, Texture* texture);
	int GetNumBone(int arrayIndex);

private:
	Matrix4f CalculateBoneMatrix(int arrayIndex, int boneIndex);
	void BoneInterporation(int arrayIndex, float ratio);

private:
	std::vector<std::vector<Bone>> boneData;
};

//本当は分離できたらよかったんだが、FBXの情報量が思ったよりも多いのでFBXManagerたちはそのまま持っておいたほうがいいかもしれない
//FBXManagerの解放責任はここにある
class FbxData {
public:
	FbxData();
	~FbxData();
	bool Initialize();
	void Add(FbxManager* manager, FbxImporter* importer, FbxScene* scene);
	FbxScene* GetScene();

private:
	FbxManager*  manager;
	FbxImporter* importer;
	FbxScene*    scene;
};

//モデルデータをひとまとめにしたもの
struct ModelDatas {
public:
	ModelDatas();
	~ModelDatas();

public:
	FbxData*       fbxData;
	VertexData*    vertexBuffer;
	MaterialData*  material;
	BoneData*      bone;
	AnimationData* animation;
};