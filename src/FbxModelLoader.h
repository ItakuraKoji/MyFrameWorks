#pragma once
#include<FBX\fbxsdk.h>
#include<Eigen\Core>
#include<vector>
#include<unordered_map>
#include<string>

#include"ModelData.h"

using namespace Eigen;

//FBXファイルから頂点、マテリアル、ボーン情報を読み取って保持し、ほかのクラスに渡す
//読み込みができなかったデータはdeleteすることによって、無い物として扱える（NULL）
//このクラスでは解放責任を負わない
//ボーンは４つ以下のウェイトで決め打ちとする　(５つ以上では不具合が出る)　のでモデル側で気を付ける
class FbxModelLoader {
private:
	struct Vertex {
		Vector3f position = Vector3f::Zero();
		Vector2f texcoord = Vector2f::Zero();
		Vector3f normal = Vector3f::Zero();
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
	bool LoadFBX(const std::string& fileName, TextureList* list);

	FbxData*       PassFbxData();
	VertexData*    PassVertexBuffer();
	MaterialData*  PassMaterialData();
	AnimationData* PassAnimationData();
	BoneData*      PassBoneData();


private:
	void          Finalize();
	bool          InitializeFBX(const std::string& fileName);
	bool          RecursiveNode(FbxNode* node);
	bool          LoadFbxMesh(FbxMesh* mesh);
	void          LoadVertex(FbxMesh* mesh, Vertex* vertex);
	void          LoadMaterial(FbxMesh* mesh, std::vector<Material>& material, std::vector<GLuint>& IBOs);
	bool          LoadBones(FbxMesh* mesh, Vertex* vertex, PolygonTable *table);

private:
	bool loaded;

	FbxData       *fbxData;
	VertexData    *bufferData;
	MaterialData  *materialData;
	AnimationData *animationData;
	BoneData      *boneData;

	//クラス内でアクセスするためのもの。deleteする責任はない
	TextureList *textureList;

	int numVertex;
	int numUV;
	int numFace;
};