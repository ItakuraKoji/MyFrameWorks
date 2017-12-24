#pragma once
#include"MeshModel.h"

//モデルの描画周りをまとめたクラス
class MeshObject{
public:
	MeshObject(MeshModel* model);
	~MeshObject();

	void Draw(GameParameters* param, Vector3f& position, Vector3f& rotation, Vector3f& scale);
	void InstanceDraw(GameParameters* param, int numDraw, Vector3f& position, Vector3f& rotation, Vector3f& scale);

protected:
	void SetMatrix(GameParameters* param, Vector3f& position, Vector3f& rotation, Vector3f& scale);

public:
	MeshModel* drawModel;
};