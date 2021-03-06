#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include<FBX\fbxsdk.h>
#include<GLEW\glew.h>
#include<Eigen\Core>
#include<Eigen\Geometry>
#include"TextureList.h"
#include"MyMathFanctions.h"

namespace K_Graphics {

	struct Material {
		std::string materialName;
		Texture* texture;
		int numFace;
		K_Math::Vector4 diffuse;
		K_Math::Vector4 ambient;
		K_Math::Vector4 specular;
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
		K_Math::Matrix4x4 bindMat;
		K_Math::Matrix4x4 currentMat;
		K_Math::Matrix4x4 interPolationMat;
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
		void SetTexture(Texture* texture, int arrayIndex, int materialIndex);
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
		void SetSpeed(float speed);
		void Add(AnimType& animData);
		void SetAnimation(const std::string& animName, FbxScene* fbxScene, bool playOnce, bool isInterpolation, bool loop);
		float GetCurrentAnimTime();

		//BoneDataにStartInterPolation()を通知する用
		bool IsStartInterpolation();

	private:
		std::unordered_map<std::string, AnimType> animList;
		float speed;
		int   currentAnimID;
		float currentAnimTime;
		int   maxAnimTime;
		bool  isLoop;
		bool  isInterpolation;
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
		void StartInterporation(int frameCount);
		int GetNumBone(int arrayIndex);

	private:
		K_Math::Matrix4x4 CalculateBoneMatrix(int arrayIndex, int boneIndex);
		void BoneInterporation(int arrayIndex, int boneIndex, float ratio);

	private:
		std::vector<std::vector<Bone>> boneData;
		float interporationCount;
		float interporationMaxCount;
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
		FbxManager * manager;
		FbxImporter* importer;
		FbxScene*    scene;
	};

	//モデルデータをひとまとめにしたもの
	struct ModelDatas {
	public:
		ModelDatas();
		~ModelDatas();

	public:
		FbxData * fbxData;
		VertexData*    vertexBuffer;
		MaterialData*  material;
		BoneData*      bone;
		AnimationData* animation;
	};
}
