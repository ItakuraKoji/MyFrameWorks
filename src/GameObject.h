#pragma once

#include"MeshModel.h"
#include"GameParameters.h"
#include"MyMathFanctions.h"

#include<Eigen\Core>
#include<Eigen\Geometry>
#include<string>

//ゲームにおける物体基底クラス、派生させてキャラクタの挙動を実現させる
//黒子を作ることも原理上は可能
//・位置、回転、拡縮
//・描画に使用するシェーダーの名前
//・描画モデル(解放責任を持つ)
class GameObject {
public:
	GameObject();
	virtual ~GameObject();
	virtual void Run(GameParameters* param) = 0;
	virtual void Draw(GameParameters* param) = 0;

	void SetDrawModel(K_Graphics::MeshObject* model);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	K_Math::Vector3 GetPosition();
	K_Math::Vector3 GetRotation();
	K_Math::Vector3 GetScale();

protected:
	K_Graphics::MeshObject* GetModel();
	void LoadModel(GameParameters& param);

protected:
	K_Math::Vector3        position;
	K_Math::Vector3        rotation;
	K_Math::Vector3        scale;
private:
	//危ないので触らせない方針で
	K_Graphics::MeshObject*      drawModel;
};