#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include<FBX\fbxsdk.h>
#include<GLEW\glew.h>
#include<Eigen\Core>
#include<Eigen\Geometry>
#include"Texture.h"

using namespace Eigen;
struct Material {
	char materialName[100];
	char textureName[100];
	int numFace;
	Texture *texture;
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
	FbxCluster *cluster;
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

//�O������ǂݍ��܂ꂽ�A���邢�͍��ꂽ�}�e���A���̏���ێ�����
//���̃N���X���ێ����Ă���e�N�X�`���̉���ӔC������
class MaterialData {
public:
	MaterialData();
	~MaterialData();
	bool Initialize();

	void Add(std::vector<Material> material);
	float GetDiffuseColor();
	float GetSpecularPower();
	float GetSpecularColor();
	float GetAmbient();
	Texture* GetTexture(int arrayIndex, int materialIndex);
	int GetNumMaterial(int arrayIndex);
	int GetNumFace(int arrayIndex, int materialIndex);

private:
	float diffuseColor;
	float ambient;
	float specularPower;
	float specularColor;
	Texture* texture;
	std::vector<std::vector<Material>> materialArray;
};

//�O������ǂݍ��܂ꂽ�A���邢�͍��ꂽ���_�o�b�t�@�̗ނ�ێ�����
//���̃N���X�́AVAO�AVBO�AIBO�A�̂R�ɂ��ĉ���ӔC������
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

//3D�A�j���[�V�������
class AnimationData {
public:
	AnimationData();
	~AnimationData();
	bool Initialize();
	void Run();
	void Add(AnimType &animData);
	void SetAnimation(std::string animName, FbxScene *fbxScene, bool playOnce, bool loop);
	int GetCurrentAnimTime();

private:
	std::unordered_map<std::string, AnimType> animList;
	int currentAnimID;
	int currentAnimTime;
	int maxAnimTime;
	bool isLoop;
	bool isInterpolation;
};

//�{�[������FBXCluster
class BoneData {
public:
	BoneData();
	~BoneData();
	bool Initialize();
	void Add(std::vector<Bone>& boneData);
	void SetClurrentBoneData(int arrayIndex, int time);
	void SetMatrixTextureData(int arrayIndex, Texture *texture);
	int GetNumBone(int arrayIndex);

private:
	Matrix4f CalculateBoneMatrix(int arrayIndex, int boneIndex);
	void BoneInterporation(int arrayIndex, float ratio);

private:
	std::vector<std::vector<Bone>> boneData;
};

//�{���͕����ł�����悩�����񂾂��AFBX���v���������f�[�^�������̂�FBXManager�����͂��̂܂܎����Ă������ق���������������Ȃ�
//FBXManager�̉���ӔC�͂����ɂ���
class FbxData {
public:
	FbxData();
	~FbxData();
	bool Initialize();
	void Add(FbxManager* manager, FbxImporter *importer, FbxScene *scene);
	FbxScene* GetScene();

private:
	FbxManager* manager;
	FbxImporter *importer;
	FbxScene *scene;
};