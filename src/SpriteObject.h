#pragma once
#include"ModelDataFactory.h"
#include"MeshModel.h"
#include"MyMathFanctions.h"

//2Dスプライト描画周りをまとめたクラス
//1辺の長さ1.0fの板ポリゴンをスプライトっぽく拡縮、UV座標を指定することができる

class SpriteObject {
public:
	SpriteObject(Texture* texture, float controlPointX = 0.0f, float controlPointY = 0.0f);
	~SpriteObject();

	bool SetTexture(Texture* texture);
	void Draw2D(CameraClass* camera, ShaderClass* shader, M::Box2D& src, M::Box2D& draw, float rotation = 0.0f);
	void Draw3D(CameraClass* camera, ShaderClass* shader, M::Box2D& src, Vector3f& position, Vector3f& rotation, Vector3f& scale);

public:
	Vector2f controlPoint;
protected:
	MeshModel* drawModel;
	void SetMatrix(CameraClass* camera, ShaderClass* shader, Vector3f& position, Vector3f& rotation, Vector3f& scale);
	Texture* cullentTexture;
};

