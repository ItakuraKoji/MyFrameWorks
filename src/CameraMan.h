#pragma once
#include"GameObject.h"

//カメラクラスと、対象となるほかのGameObjectへのポインタを持つカメラマン
//ターゲットから一定距離を保って動く
class CameraMan : public GameObject {
public:
	bool Initialize(float distance, float height);
	void Finalize();
	void Run(GameParameters* param);
	void Draw(GameParameters* param);

	void SetParameter(GameObject* terget, CameraClass* camera);
	void SetRotation(float degHorizontal, float degVertical);
	float GetRotationH();
	float GetRotationV();

private:
	GameObject* terget;
	CameraClass* camera;
	float distance;
	float height;
	float camerarotH, camerarotV;

};