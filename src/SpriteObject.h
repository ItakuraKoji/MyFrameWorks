#pragma once
#include"ModelDataFactory.h"
#include"MeshModel.h"
#include"MyMathFanctions.h"

//2D�X�v���C�g�`�������܂Ƃ߂��N���X
//1�ӂ̒���1.0f�̔|���S�����X�v���C�g���ۂ��g�k�AUV���W���w�肷�邱�Ƃ��ł���

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

