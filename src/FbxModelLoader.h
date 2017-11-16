#pragma once
#include<FBX\fbxsdk.h>
#include<Eigen\Core>
#include<vector>
#include<unordered_map>

#include"ModelData.h"

using namespace Eigen;

//FBXファイルから頂点、マテリアル、ボーン情報を読み取って保持し、ほかのクラスに渡す
//渡さなかったクラスはこのクラスで開放する
//ボーンは４つ以下のウェイトで決め打ちとする　(５つ以上では不具合が出る)　のでモデル側で気を付ける
class FbxModelLoader {
private:
	struct Vertex {
		Vector3f position;
		Vector2f texcoord;
		Vector3f normal;
		Vector4f boneWeight = Vector4f::Zero();
		Vector4i boneIndex = Vector4i::Zero();
	};

	//頂点からポリゴンを逆引きするテーブル
	struct PolygonTable {
		int polygonIndex[255] = {};
		int polygon123[255] = {};
		int numPolygon = 0;
	};


public:
	FbxModelLoader();
	~FbxModelLoader();
	bool Initialize(const char* fileName);

	FbxData*       PassFbxData();
	VertexData*    PassVertexBuffer();
	MaterialData*  PassMaterialData();
	AnimationData* PassAnimationData();
	BoneData*      PassBoneData();


private:
	bool          InitializeFBX(const char* fileName);
	bool          RecursiveNode(FbxNode* node);
	bool          LoadFbxMesh(FbxMesh* mesh);
	void          LoadVertex(FbxMesh* mesh, Vertex* vertex);
	void          LoadMaterial(FbxMesh* mesh, std::vector<Material>& material, std::vector<GLuint>& IBOs);
	bool          LoadBones(FbxMesh* mesh, Vertex* vertex, PolygonTable *table);
	PolygonTable* CreatePolygonTable(FbxMesh *mesh, int numVertex, int numFace);


private:
	FbxData       *fbxData;
	VertexData    *bufferData;
	MaterialData  *materialData;
	AnimationData *animationData;
	BoneData      *boneData;
};