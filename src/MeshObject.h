#pragma once
#include"MeshModel.h"

//3Dƒ‚ƒfƒ‹‚Ì•`‰æŽü‚è‚ð‚Ü‚Æ‚ß‚½ƒNƒ‰ƒX
class MeshObject {
public:
	MeshObject(MeshModel* model);
	~MeshObject();

	void SetBoneAnimation(const std::string& animationName, bool playOnce, bool isLoop, bool isInterporation = true, int interpolationFrames = 5);
	void SetSpeed(float speed);
	
	void UpdateAnimation();

	void Draw(CameraClass* camera, ShaderClass* shader, Vector3f& position, Vector3f& rotation, Vector3f& scale);
	void InstanceDraw(CameraClass* camera, ShaderClass* shader, int numDraw, Vector3f& position, Vector3f& rotation, Vector3f& scale);

protected:
	void SetMatrix(CameraClass* camera, ShaderClass* shader, Vector3f& position, Vector3f& rotation, Vector3f& scale);
	MeshModel* drawModel;
};