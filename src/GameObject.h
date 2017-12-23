#pragma once

#include"MeshObject.h"
#include"DrawParameters.h"
#include"MyMathFanctions.h"

#include<Eigen\Core>
#include<Eigen\Geometry>
#include<string>

using namespace Eigen;

//ゲームにおける物体基底クラス、派生させてキャラクタの挙動を実現させる
//黒子を作ることも原理上は可能
//・位置、回転、拡縮
//・描画に使用するシェーダーの名前
//・描画モデル(解放責任を持つ)
class GameObject {
public:
	GameObject();
	virtual ~GameObject();
	virtual void Run(GameParameters& param) = 0;
	virtual void Draw(GameParameters& param) = 0;

	void SetDrawModel(MeshObject* model);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	Vector3f GetPosition();
	Vector3f GetRotation();
	Vector3f GetScale();

protected:
	MeshObject* GetModel();
	void LoadModel(GameParameters& param);

protected:
	Vector3f        position;
	Vector3f        rotation;
	Vector3f        scale;
private:
	//危ないので触らせない方針で
	MeshObject*      drawModel;
};