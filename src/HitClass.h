#pragma once

#include"HitPrimitive.h"

//�����蔻��
namespace Hit {
	//�v�Z
	//0�`1�͈̔͂�float�̒l�����߂ĕԂ�
	void  Clamp01(float &v);
	//���ʂƓ_�̋���
	float GetDistancePointAndPlane(const Vector3f &point, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3);
	//�|���S���̖@�����擾
	Vector3f GetPolygonNormal(const Vector3f &point1, const Vector3f &point2, const Vector3f &point3);
	//��_�Ԃ̋������Z�o
	float GetDistancePointAndPoint(const Vector3f &p1, const Vector3f &p2);
	//�_�ƒ����̋���
	float GetDistancePointAndLine(const Vector3f &p, const Line &l, Vector3f &h, float &t);
	//�_�Ɛ����̋���
	float GetDistancePointAndSegment(const Vector3f &p, const Segment &s, Vector3f &h, float &t);
	//�����ƒ����̋����A�����̑��̍��W�Ǝn�_����̃x�N�g���W����Ԃ�
	float GetDistanceLineAndLine(const Line &line1, const Line &line2, Vector3f &pn1, Vector3f &pn2, float &t1, float &t2);
	//�����Ɛ����̋���
	float GetDistanceSegmentAndSegment(const Segment &seg1, const Segment &seg2);
	//�����ƃ|���S���̋���
	float GetDistanceSegmentAndPolygon(const Segment &seg, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3);

	//����
	//�|���S���ƃJ�v�Z��
	bool IsHitPolygonAndCapsule(const Capsule &capsule, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, Vector3f &crossPoint);
	//�|���S���Ɛ����̏Փ�
	bool HitSegmentAndPolygon(const Segment &segment, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, Vector3f &crossPoint);
	//�J�v�Z���ƃJ�v�Z��
	bool HitCapsuleAndCapsule(const Capsule &c1, const Capsule &c2);
	//�|���S�����ɓ_���܂܂�邩
	bool isPointinPolygon(const Vector3f &p, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3);
	//�����ƕ��ʂ̏Փ�
	bool isHitSegmentAndPlane(const Segment &segment, const Vector3f &polygon, const Vector3f &p2, const Vector3f &p3);
}