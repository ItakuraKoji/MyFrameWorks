#include"Player.h"

bool Player::Initialize(GameParameters& param) {
	this->velocity = 0.0f;
	this->camerarotH = 0.0f;
	this->camerarotV = 0.0f;

	btCollisionShape *characterShape = param.physicsSystem->CreateCapsuleShape(0.3f, 0.8f);
	this->characterCollision = param.physicsSystem->CreateRigidBody(characterShape, 0.0f, 1, 1, btVector3(this->position.x(), this->position.y(), this->position.z()));

	this->GetModel()->SetAnimation("Dash", true, false, false);
	return true;
}
void Player::Finalize() {

}
void Player::Run(GameParameters& param) {

	float srRotation = param.input->GetStickRotation(VPAD_STICK_R);
	float srPower = param.input->GetStickPower(VPAD_STICK_R);

	camerarotH += 2.0f * srPower * cosf(srRotation);
	camerarotV -= 2.0f * srPower * sinf(srRotation);

	float slRotation = param.input->GetStickRotation(VPAD_STICK_L);
	float slPower = param.input->GetStickPower(VPAD_STICK_L);
	if (slPower) {
		this->rotation.y() = slRotation + DegToRad(90.0f) + DegToRad(camerarotH);
	}

	Vector3f direction(0.0f, 0.0f, 0.0f);
	direction.x() += 0.4f * slPower * cosf(slRotation);
	direction.z() -= 0.4f * slPower * sinf(slRotation);

	if (param.input->isPressButton(VPAD_BUTTON_A)) {
		this->velocity = 1.0f;
	}
	this->velocity -= param.gravity;
	if (this->velocity < -1.0f) {
		this->velocity = -1.0f;
	}
	direction.y() += this->velocity;


	//移動ベクトルをカメラの軸に合わせる
	Vector3f xAxis = param.camera->GetAxisX();
	Vector3f zAxis = param.camera->GetAxisZ();
	xAxis.y() = 0.0f;
	zAxis.y() = 0.0f;
	Vector3f goVec = direction.x() * xAxis.normalized() + Vector3f(0, direction.y(), 0) + direction.z() * zAxis.normalized();

	goVec /= 1.0f;
	for (int i = 0; i < 1; ++i) {
		param.physicsSystem->MoveCharacterObject(this->characterCollision, btVector3(goVec.x(), 0.0f, goVec.z()), btVector3(0.0f, goVec.y(), 0.0f));
		//param.physicsSystem->DiscreteMoveObject(this->characterCollision, btVector3(goVec.x(), 0.0f, goVec.z()), btVector3(0.0f, goVec.y(), 0.0f));
	}
	btTransform trans = this->characterCollision->getWorldTransform();
	this->position.x() = trans.getOrigin().x();
	this->position.y() = trans.getOrigin().y() - 0.75f;
	this->position.z() = trans.getOrigin().z();
	btRigidBody::upcast(this->characterCollision)->activate(true);

	float cx = -20.0f * sinf(camerarotH * (float)M_PI / 180.0f) + this->position.x();
	float cy = -20.0f * sinf(camerarotV * (float)M_PI / 180.0f) + this->position.y();
	float cz = -20.0f * cosf(camerarotH * (float)M_PI / 180.0f) * cosf(camerarotV * (float)M_PI / 180.0f) + this->position.z();
	param.camera->SetPosition(cx, cy + 4.0f, cz);
	param.camera->SetTarget(this->position.x(), this->position.y() + 4.0f, this->position.z());
}

void Player::Draw(GameParameters& param) {
	//return;
	param.camera->Draw();
	ShaderClass* shader = param.shaderList->UseShader(this->shaderName);
	param.lightList->SetAmbient("ambient", shader);
	param.lightList->SetDirectional("directional", shader);
	shader->SetVertexShaderSubroutine("CalcBoneMat");
	SetMatrix(param);
	this->GetModel()->Draw(param, this->shaderName);
}