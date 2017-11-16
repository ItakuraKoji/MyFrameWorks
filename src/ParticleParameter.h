#pragma once
#include<Eigen\Core>
using namespace Eigen;

//キーフレームは未実装
//キーフレームで扱う時間は　0.0f～1.0f　の間とする

//エミッタのパラメーター、パーティクルの初期値やその生成量などの値を持つ
struct EmitterParameter {
	int firstLifeLimit;
	int emitQuantity;
	int emitInterval;

	//particleの初期値
	//position
	Vector3f firstPosition;
	Vector3f firstUV;
	Vector2f firstScale;
	//velosity
	Vector3f firstPositionVelosity;
	Vector3f firstUVVelosity;
	Vector3f firstScaleVelosity;
	//color(KeyFrame)
	Vector4f firstColor;
	//std::vector<Vector4f> keyFrameColor;
};

//パーティクルのパラメーター、位置、UV座標、サイズや速度や寿命の値などを持つ
struct ParticleParameter {
	int lifeLimit;
	//position
	Vector3f position;
	Vector3f uv;
	Vector2f scale;
	//velosity
	Vector3f positionVelosity;
	Vector3f uvVelosity;
	Vector3f scaleVelosity;
	//color(KeyFrame)
	Vector4f firstColor;
	//std::vector<Vector4f> keyFrameColor;
};