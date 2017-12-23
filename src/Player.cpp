#include"Player.h"

bool Player::Initialize(GameParameters& param) {
	this->velocity = 0.0f;
	this->speed = 0.4f;

	btCollisionShape *characterShape = param.physicsSystem->CreateCapsuleShape(0.3f, 0.8f);
	this->characterCollision = param.physicsSystem->CreateRigidBody(characterShape, 0.0f, 1, 1, btVector3(this->position.x(), this->position.y(), this->position.z()));

	this->GetModel()->drawModel->SetAnimation("Dash", true, false, false);
	return true;
}
void Player::Finalize() {
	delete this->camera;
}
void Player::Run(GameParameters& param) {
	float slRotation = param.input->GetStickRotation(VPAD_STICK_L);
	float slPower    = param.input->GetStickPower(VPAD_STICK_L);

	//アナログが倒されている方向 カメラの回転 に対応したキャラの向き
	if (slPower) {
		this->rotation.y() = slRotation + DegToRad(-90.0f) + this->camera->GetRotationH();
	}

	Vector3f direction(0.0f, 0.0f, 0.0f);
	direction.x() += this->speed * slPower * cosf(slRotation);
	direction.z() -= this->speed * slPower * sinf(slRotation);

	if (param.input->isPressButton(VPAD_BUTTON_A)) {
		this->velocity = 1.0f;
	}
	this->velocity -= param.gravity;
	if (this->velocity < -1.0f) {
		this->velocity = -1.0f;
	}
	direction.y() += this->velocity;


	//移動ベクトルを指定したカメラの軸に合わせる
	Vector3f goVec = AngleAxisf(this->camera->GetRotationH(), Vector3f(0.0f, 1.0f, 0.0f)) * direction;

	param.physicsSystem->MoveCharacterObject(this->characterCollision, btVector3(goVec.x(), 0.0f, goVec.z()), btVector3(0.0f, goVec.y(), 0.0f));
	//param.physicsSystem->DiscreteMoveObject(this->characterCollision, btVector3(goVec.x(), 0.0f, goVec.z()), btVector3(0.0f, goVec.y(), 0.0f));
	
	btTransform trans = this->characterCollision->getWorldTransform();
	this->position.x() = trans.getOrigin().x();
	this->position.y() = trans.getOrigin().y() - 0.75f;
	this->position.z() = trans.getOrigin().z();
	this->characterCollision->activate(true);

	//カメラ操作はカメラマンが行う
	this->camera->Run(param);
}

void Player::Draw(GameParameters& param) {
	this->GetModel()->Draw(param, this->position, this->rotation, this->scale);
}

void Player::SetCameraMan(CameraClass* camera) {
	this->camera = new CameraMan;
	this->camera->SetParameter(this, camera);
}
