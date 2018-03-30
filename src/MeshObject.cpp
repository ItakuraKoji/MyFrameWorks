#include"MeshModel.h"

namespace K_Graphics {

	////////
	//public
	////

	MeshObject::MeshObject(MeshModel* model) {
		if (model == nullptr) {
			throw("modelData is nullptr");
		}
		this->drawModel = model;
	}
	MeshObject::~MeshObject() {
		if (this->drawModel != nullptr) {
			delete this->drawModel;
			this->drawModel = nullptr;
		}
	}

	void MeshObject::SetBoneAnimation(const std::string& animationName, bool playOnce, bool isLoop, bool isInterporation, int interpolationFrames) {
		this->drawModel->SetAnimation(animationName, playOnce, isLoop, isInterporation, interpolationFrames);
	}
	void MeshObject::SetSpeed(float speed) {
		this->drawModel->SetSpeed(speed);
	}

	void MeshObject::UpdateAnimation() {
		this->drawModel->UpdateAnimation();
	}

	void MeshObject::Draw(CameraClass* camera, ShaderClass* shader, K_Math::Vector3& position, K_Math::Vector3& rotation, K_Math::Vector3& scale) {
		SetMatrix(camera, shader, position, rotation, scale);
		this->drawModel->Draw(shader);
	}
	void MeshObject::InstanceDraw(CameraClass* camera, ShaderClass* shader, int numDraw, K_Math::Vector3& position, K_Math::Vector3& rotation, K_Math::Vector3& scale) {
		SetMatrix(camera, shader, position, rotation, scale);
		this->drawModel->InstanceDraw(numDraw, shader);
	}

	////////
	//protected
	////

	void MeshObject::SetMatrix(CameraClass* camera, ShaderClass* shader, K_Math::Vector3& position, K_Math::Vector3& rotation, K_Math::Vector3& scaling) {
		//ˆÚ“®
		K_Math::Translation trans = K_Math::Translation(position);
		//‰ñ“]‡‚ÍYXZ
		K_Math::Quaternion rot;
		rot = K_Math::AngleAxis(0, K_Math::Vector3::Zero());
		rot = rot * K_Math::AngleAxis(rotation.y(), K_Math::Vector3::UnitY());
		rot = rot * K_Math::AngleAxis(rotation.x(), K_Math::Vector3::UnitX());
		rot = rot * K_Math::AngleAxis(rotation.z(), K_Math::Vector3::UnitZ());
		//ƒXƒP[ƒ‹
		K_Math::DiagonalMatrix scale = K_Math::DiagonalMatrix(K_Math::Vector3(-scaling.x(), scaling.y(), scaling.z()));


		K_Math::Affine3 world = trans * rot * scale;
		K_Math::Matrix4x4 view = camera->GetViewMatrix();
		K_Math::Matrix4x4 projection = camera->GetProjectionMatrix();

		shader->SetMatrix(projection * view * world.matrix());
		shader->SetWorldMatrix(world.matrix());
	}

}
