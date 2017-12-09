#pragma once
#include<FBX\fbxsdk.h>
#include<Eigen\Core>
#include<vector>
#include<unordered_map>
#include<string>

#include"ModelData.h"

using namespace Eigen;

//FBX�t�@�C�����璸�_�A�}�e���A���A�{�[������ǂݎ���ĕێ����A�ق��̃N���X�ɓn��
//�ǂݍ��݂��ł��Ȃ������f�[�^��delete���邱�Ƃɂ���āA�������Ƃ��Ĉ�����iNULL�j
//���̃N���X�ł͉���ӔC�𕉂�Ȃ�
//�{�[���͂S�ȉ��̃E�F�C�g�Ō��ߑł��Ƃ���@(�T�ȏ�ł͕s����o��)�@�̂Ń��f�����ŋC��t����
class FbxModelLoader {
private:
	struct Vertex {
		Vector3f position;
		Vector2f texcoord;
		Vector3f normal;
		Vector4f boneWeight = Vector4f::Zero();
		Vector4i boneIndex = Vector4i::Zero();
	};

	//���_����|���S�����t��������e�[�u��
	struct PolygonTable {
		int polygonIndex[255] = {};
		int polygon123[255] = {};
		int numPolygon = 0;
	};


public:
	FbxModelLoader();
	~FbxModelLoader();
	bool LoadFBX(const std::string& fileName, TextureList* list);

	FbxData*       PassFbxData();
	VertexData*    PassVertexBuffer();
	MaterialData*  PassMaterialData();
	AnimationData* PassAnimationData();
	BoneData*      PassBoneData();


private:
	bool          InitializeFBX(const std::string& fileName);
	bool          RecursiveNode(FbxNode* node);
	bool          LoadFbxMesh(FbxMesh* mesh);
	void          LoadVertex(FbxMesh* mesh, Vertex* vertex);
	void          LoadMaterial(FbxMesh* mesh, std::vector<Material>& material, std::vector<GLuint>& IBOs);
	bool          LoadBones(FbxMesh* mesh, Vertex* vertex, PolygonTable *table);
	PolygonTable* CreatePolygonTable(FbxMesh *mesh, int numVertex, int numFace);


private:
	bool loaded;

	FbxData       *fbxData;
	VertexData    *bufferData;
	MaterialData  *materialData;
	AnimationData *animationData;
	BoneData      *boneData;

	//�N���X���ŃA�N�Z�X���邽�߂̂��́Bdelete����ӔC�͂Ȃ�
	TextureList *textureList;
};