#include"ModelData.h"

////////
//public
////
AnimationData::AnimationData() {
	this->speed = 1;
	this->currentAnimID = 0;
	this->currentAnimTime = 0;
	this->isLoop = false;
	this->isInterpolation = false;
}
AnimationData::~AnimationData() {

}

bool AnimationData::Initialize() {

	return true;
}

void AnimationData::UpdateAnimation() {
	this->currentAnimTime += this->speed * 2;
	if (this->currentAnimTime > this->maxAnimTime) {
		if (this->isLoop) {
			this->currentAnimTime = 0;
		}
	}
}

void AnimationData::Add(AnimType &animData) {
	this->animList[animData.animName] = animData;
}

void AnimationData::SetSpeed(int speed) {
	this->speed = speed;
}

//�A�j���[�V�������Z�b�g
void AnimationData::SetAnimation(const std::string& animName, FbxScene *fbxScene, bool playOnce, bool loop) {
	//�L�[�����݂��Ȃ��Ȃ�A��
	if (this->animList.find(animName) == this->animList.end()) {
		return;
	}

	AnimType anim = this->animList[animName];
	//�A���Đ��i�����A�j���[�V�������Đ��j�������Ă��Ȃ��ꍇ
	if (this->currentAnimID == anim.animID && playOnce) {
		return;
	}
	this->currentAnimTime = 0;
	this->currentAnimID = anim.animID;
	this->maxAnimTime = anim.endTime - anim.startTime;
	FbxAnimStack *pStack = fbxScene->GetSrcObject<FbxAnimStack>(this->currentAnimID);
	fbxScene->SetCurrentAnimationStack(pStack);

	this->isLoop = loop;
	this->isInterpolation = true;
}

int AnimationData::GetCurrentAnimTime() {
	return this->currentAnimTime;
}