#pragma once

#include"MeshModel.h"
#include"DrawParameters.h"

#include<Eigen\Core>
#include<string>

using namespace Eigen;

//ゲームにおける描画対象キャラクタ基底クラス、派生させてキャラクタの挙動を実現させる
//・位置、回転、拡縮
//・描画に使用するシェーダーの名前
//・描画モデル
class GameObject {
public:
	GameObject();
	virtual ~GameObject();
	virtual bool Initialize(DrawParameters& param) = 0;
	virtual void Finalize() = 0;
	virtual void Run(DrawParameters& param) = 0;
	virtual void Draw(DrawParameters& param) = 0;

	void SetDrawModel(MeshModel* model);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	Vector3f GetPosition();
	Vector3f GetRotation();
	Vector3f GetScale();

protected:
	void SetMatrix(DrawParameters& param);

protected:
	Vector3f        position;
	Vector3f        rotation;
	Vector3f        scale;
	std::string     shaderName;
	MeshModel*      drawModel;
};