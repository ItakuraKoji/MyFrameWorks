#pragma once
#include"MeshModel.h"

//ƒ‚ƒfƒ‹‚Ì•`‰æŽü‚è‚ð‚Ü‚Æ‚ß‚½ƒNƒ‰ƒX
class MeshObject{
public:
	MeshObject(MeshModel* model);
	~MeshObject();

	void Draw(GameParameters& param, Vector3f& position, Vector3f& rotation, Vector3f& scale);
	void InstanceDraw(GameParameters& param, int numDraw, Vector3f& position, Vector3f& rotation, Vector3f& scale);

protected:
	void SetMatrix(GameParameters& param, Vector3f& position, Vector3f& rotation, Vector3f& scale);

public:
	MeshModel* drawModel;
};