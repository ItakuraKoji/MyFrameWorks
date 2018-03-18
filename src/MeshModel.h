#pragma once

#include"FbxModelLoader.h"
#include"ModelData.h"
#include"GameParameters.h"
#include"ShaderClass.h"

//モデルクラス。ファクトリーから生産されたモデルデータを受け取って初期化
//「形状」という扱いなので、描画時はモデルデータ以外の必要な情報を外から渡すものとする
class MeshModel {
public:
	MeshModel(ModelDatas* data);
	~MeshModel();

	bool Initialize(ModelDatas* data);
	void Finalize();
	void SetAnimation(const std::string& animationName, bool playOnce, bool isLoop, bool isInterporation, int interpolationFrames);
	void SetTexture(Texture* texture, int arrayIndex, int materialIndex);
	void SetSpeed(float speed);

	std::string& SetTextureName(int arrayIndex, int materialIndex);


	void UpdateAnimation();
	void Draw(GameParameters* param);
	void InstanceDraw(int numInstance, GameParameters* param);

private:
	void SetBone    (int arrayIndex, GameParameters* param);
	void DrawBuffers(int arrayIndex, GameParameters* param);

private:
	ModelDatas* data;
	Texture*  boneTexture;
	bool isBoneProcessed;

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};