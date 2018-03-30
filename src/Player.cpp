#include"Player.h"
float collitionFix = 1.2f;

bool Player::Initialize(GameParameters* param) {
	this->velocity = 0.0f;
	this->speed = 0.6f;
	SetPosition(0, 30, 0);

	btCollisionShape *characterShape = param->GetPhysics()->CreateCapsuleShape(0.4f, 1.6f);
	this->characterCollision = param->GetPhysics()->CreateCollisionObject(characterShape, false, 1, btVector3(this->position.x(), this->position.y() + collitionFix, this->position.z()));
	btCollisionShape *sphereShape = param->GetPhysics()->CreateSphereShape(0.2f);
	this->characterFoot = param->GetPhysics()->CreateCollisionObject(sphereShape, true, 1, this->characterCollision->collision->getWorldTransform().getOrigin() + btVector3(0.0f, 0.0f, 0.0f));
	this->characterFoot->collision->setCustomDebugColor(btVector3(1.0f, 0.0f, 0.0f));

	this->GetModel()->SetBoneAnimation("Idle", true, true, false);
	this->GetModel()->SetSpeed(1.0f);
	return true;
}
void Player::Finalize() {
	delete this->camera;
}
void Player::Run(GameParameters* param) {
	this->characterCollision->SetCollisionPosition(btVector3(this->position.x(), this->position.y(), this->position.z()) + btVector3(0.0f, collitionFix, 0.0f));
	this->characterFoot->SetCollisionPosition(this->characterCollision->GetCollisionPosition() + btVector3(0.0f, -1.1f, 0.0f));

	//足元の着地判定
	bool isGround = false;
	std::vector<K_Physics::CollisionTag>& tags = param->GetPhysics()->FindConfrictionObjects(this->characterFoot->collision);
	if (tags.size()) {
		isGround = true;
	}

	float slRotation = param->GetInput()->GetStickRotation(K_Input::VPAD_STICK_L);
	float slPower    = param->GetInput()->GetStickPower(K_Input::VPAD_STICK_L);

	//アナログが倒されている方向 カメラの回転 に対応したキャラの向き
	if (slPower) {
		this->rotation.y() = -slRotation + K_Math::DegToRad(-90.0f) + this->camera->GetRotationH();
		if (slPower > 0.65f) {
			if (isGround) {
				this->GetModel()->SetBoneAnimation("Dash", true, true, true, 10);
				this->GetModel()->SetSpeed(1.0f + (slPower - 0.65f) * 0.65f);
			}
			this->speed = 0.6f;
		}
		else {
			if (isGround) {
				this->GetModel()->SetBoneAnimation("Walk", true, true, true, 5);
				this->GetModel()->SetSpeed(1.0f + slPower / 0.65f);
			}
			this->speed = 0.2f;
		}
	}
	else {
		if (isGround) {
			this->GetModel()->SetBoneAnimation("Idle", true, true, true);
			this->GetModel()->SetSpeed(1.0f);
		}
	}

	K_Math::Vector3 direction(0.0f, 0.0f, 0.0f);
	direction.x() += this->speed * slPower * cosf(slRotation);
	direction.z() += this->speed * slPower * sinf(slRotation);



	if (param->GetInput()->isPressButton(K_Input::VPAD_BUTTON_A)) {
		this->velocity = 1.0f;
		this->GetModel()->SetBoneAnimation("Jump", false, false, true, 3);
		this->GetModel()->SetSpeed(0.8f);
		isGround = false;
	}

	if (param->GetInput()->isPressButton(K_Input::VPAD_BUTTON_B)) {
		param->GetEffects()->Play("test", this->position.x(), this->position.y(), this->position.z());
		//param->GetAudioList()->GetSource("se")->PlayCopy();
	}

	this->velocity -= 0.03f;
	if (this->velocity < -1.0f) {
		//this->velocity = -1.0f;
	}
	if (isGround) {
		this->velocity = -0.1f;
	}
	direction.y() += this->velocity;


	//移動ベクトルを指定したカメラの軸に合わせる
	K_Math::Vector3 goVec = K_Math::AngleAxis(this->camera->GetRotationH(), K_Math::Vector3(0.0f, 1.0f, 0.0f)) * direction;

	param->GetPhysics()->MoveCharacter(this->characterCollision->collision, btVector3(goVec.x(), goVec.y(), goVec.z()));
	//param->GetPhysics()->MoveCharacter(this->characterCollision->collision, btVector3(goVec.x(), 0.0f, goVec.z()), btVector3(0.0f, goVec.y(), 0.0f));
	//param->GetPhysics()->MoveCharacterDiscrete(this->characterCollision->collision, btVector3(goVec.x(), 0.0f, goVec.z()), btVector3(0.0f, goVec.y(), 0.0f));
	
	btTransform trans = this->characterCollision->collision->getWorldTransform();
	this->position.x() = trans.getOrigin().x();
	this->position.y() = trans.getOrigin().y() - collitionFix;
	this->position.z() = trans.getOrigin().z();

	trans.setOrigin(trans.getOrigin() + btVector3(0.0f, -1.1f, 0.0f));
	this->characterFoot->collision->setWorldTransform(trans);

	//カメラ操作はカメラマンが行う
	this->camera->Run(param);
	//アニメーション更新
	this->GetModel()->UpdateAnimation();
	//system("pause");
	
	//param->GetFontRenderer()->DrawString3D("X : " + std::to_string(this->position.x()), 32, this->position.x(), this->position.y(), this->position.z());
	//param->GetFontRenderer()->DrawString3D("Y : " + std::to_string(this->position.y()), 32, this->position.x(), this->position.y() + 32 * 0.05f, this->position.z());
	//param->GetFontRenderer()->DrawString3D("Z : " + std::to_string(this->position.z()), 32, this->position.x(), this->position.y() + 64 * 0.05f, this->position.z());
	param->GetFontRenderer()->DrawString3D("X : " + std::to_string(this->position.x()), 32, 0.0f, 0.0f, -600.0f);
	param->GetFontRenderer()->DrawString3D("Y : " + std::to_string(this->position.y()), 32, 0.0f, 0.0f + 32 * 0.05f, -600.0f);
	param->GetFontRenderer()->DrawString3D("Z : " + std::to_string(this->position.z()), 32, 0.0f, 0.0f + 64 * 0.05f, -600.0f);
}

void Player::Draw(GameParameters* param) {
	this->GetModel()->Draw(param->currentCamera, param->currentShader, this->position, this->rotation, this->scale);
}

void Player::SetCameraMan(K_Graphics::CameraClass* camera) {
	this->camera = new CameraMan;
	this->camera->Initialize(20.0f, 5.0f);
	this->camera->SetRotation(-180, 0);
	this->camera->SetParameter(this, camera);
}

////////
//private
////

