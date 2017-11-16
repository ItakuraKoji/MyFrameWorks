#include"HitClass.h"

namespace Hit {
	//�����ƃ|���S���̋���(NG)
	float GetDistanceSegmentAndPolygon(const Segment &seg, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3) {
		//�����ƃ|���S�����������Ă���ꍇ�̋����͂O
		Vector3f crossPoint;
		if (HitSegmentAndPolygon(seg, p1, p2, p3, crossPoint)) {
			return 0.0f;
		}

		float returnLength = 0.0f;
		Vector3f verticalFoot;
		Vector3f firstPoint = seg.firstPoint;
		Vector3f finalPoint = seg.GetFinalPoint();

		//���[����|���S�����܂ޕ��ʂ̋��������A�����̑����|���S�����Ȃ炻�̋�����������
		float length1 = GetDistancePointAndPlane(firstPoint, p1, p2, p3);
		verticalFoot = firstPoint - length1 * GetPolygonNormal(p1, p2, p3);
		if (!isPointinPolygon(verticalFoot, p1, p2, p3)) {
			length1 = -1.0f;
		}

		float length2 = GetDistancePointAndPlane(finalPoint, p1, p2, p3);
		verticalFoot = finalPoint - length2 * GetPolygonNormal(p1, p2, p3);
		if (!isPointinPolygon(verticalFoot, p1, p2, p3)) {
			length2 = -1.0f;
		}

		if (length1 < length2 && length1 > 0.0f) {
			return length1;
		}
		else if (length2 > 0.0f) {
			return length2;
		}

		//����Ƀ|���S���̎O�_������Ă��Z���������̗p
		float length3;
		length3 = GetDistanceSegmentAndSegment(seg, Segment(p1, Vector3f(p2 - p1)));
		returnLength = length3;

		length3 = GetDistanceSegmentAndSegment(seg, Segment(p2, Vector3f(p3 - p2)));
		if (length3 < returnLength) {
			returnLength = length3;
		}
		length3 = GetDistanceSegmentAndSegment(seg, Segment(p3, Vector3f(p1 - p3)));
		if (length3 < returnLength) {
			returnLength = length3;
		}


		return returnLength;
	}
	//�|���S���ƃJ�v�Z���̏Փ�(NG)
	//capsule : ������Ƃ�J�v�Z��
	//p1, p2, p3 : �|���S����3�_
	//crossPoint : �Փ˓_
	bool IsHitPolygonAndCapsule(const Capsule &capsule, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, Vector3f &crossPoint) {
		float length = GetDistanceSegmentAndPolygon(capsule.segment, p1, p2, p3);
		if (length > capsule.radius) {
			return false;
		}

		return true;
	}



	//�|���S���Ɛ����̏Փ�
	//segment, p1, p2, p3: ����
	//crossPoint: �����o�����W�i�߂�l�j
	bool HitSegmentAndPolygon(const Segment &segment, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, Vector3f &crossPoint) {
		//�|���S�����܂ޕ��ʂƐ������Փ˂��Ă��邩�𒲂ׂ�
		if (!isHitSegmentAndPlane(segment, p1, p2, p3)) {
			return false;
		}

		//�����ƃ|���S���̓�����
		float s;
		//���ʂƐ����̌�_���W
		Vector3f point;
		//�n�_�ƏI�_���畽�ʂւ̋���
		float d1, d2;

		d1 = GetDistancePointAndPlane(segment.firstPoint, p1, p2, p3);
		d2 = GetDistancePointAndPlane(segment.GetFinalPoint(), p1, p2, p3);

		//�����ƕ��ʂ̌�_�̓�����s�����߂�
		//�i�n�_�����_�F��_����I�_�@���@d1�Fd2�@���@s�F(1-s) �j
		if (d1 + d2) {
			s = d1 / (d1 + d2);
		}
		else {
			s = 0;
		}

		//�����䂩����W���Z�o
		point = segment.firstPoint + segment.vector * s;

		//�Z�o�����_���|���S�����Ɋ܂܂�邩�𒲂ׂ�
		if (!isPointinPolygon(point, p1, p2, p3)) {
			return false;
		}

		//�Փ˒n�_���班���߂����n�_�������蔻��̉����߂��l�Ƃ��ĕԂ�
		crossPoint = segment.firstPoint + segment.vector * (s - 0.01f);
		return true;
	}

	//�J�v�Z���ƃJ�v�Z���̏Փ�(NG?)
	//c1, c2	: ����
	bool HitCapsuleAndCapsule(const Capsule &c1, const Capsule &c2) {
		float len = GetDistanceSegmentAndSegment(c1.segment, c2.segment);
		float radius = c1.radius + c2.radius;
		if (len > radius) {
			return false;
		}
		return true;
	}

	//���ʂƐ����̏Փ�
	//segment		: ���͒l
	//p1, p2, p3	: ���ʏ�̎O�_
	bool isHitSegmentAndPlane(const Segment &segment, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3) {
		//�ʂ̖@�����Z�o
		Vector3f normal = GetPolygonNormal(p1, p2, p3);

		//���������ʂɊ܂܂�Ă��邩(�@���Ɛ�����������)�ǂ����𒲂ׂ�
		if (normal.dot(segment.vector) == 0.0f && normal.dot(p1 - segment.firstPoint) != 0.0f) {
			return false;
		}

		//�������������Ă��邩�𔻒�
		float result = normal.dot(segment.firstPoint - p1) * normal.dot(segment.GetFinalPoint() - p1);
		if (result > 0) {
			return false;
		}
		return true;
	}




	//�����ʏ�̎O�p�|���S�����ɓ_���܂܂�邩�ǂ���
	//p				: �C�ӂ̓_
	//p1, p2, p3	: �|���S���̎O�_�i�����v��肪�\�j
	bool isPointinPolygon(const Vector3f &p, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3) {
		//�Ӄx�N�g��
		Vector3f v1 = p2 - p1;
		Vector3f v2 = p3 - p2;
		Vector3f v3 = p1 - p3;
		Vector3f normal = GetPolygonNormal(p1, p2, p3);
		Vector3f result;

		//�@���ƊO�ςœ���ꂽ�x�N�g�������ׂē����Ȃ�_���|���S�����ɂ���
		result = v1.cross(p - p1);
		if (result.dot(normal) < 0.0f) {
			return false;
		}

		result = v2.cross(p - p2);
		if (result.dot(normal) < 0.0f) {
			return false;
		}

		result = v3.cross(p - p3);
		if (result.dot(normal) < 0.0f) {
			return false;
		}

		return true;
	}

	//��_�̋���
	float GetDistancePointAndPoint(const Vector3f &p1, const Vector3f &p2) {
		return (p2 - p1).norm();
	}

	//�_�ƒ����̋���(NG)
	//point, line	: ���͒l
	//h				: �_���璼���ɑ΂��鐂���̑��̍��W�i�߂�l�j
	//t				: �n�_���琂���̑��ւ̃x�N�g���W���i�߂�l�j
	float GetDistancePointAndLine(const Vector3f &point, const Line &line, Vector3f &h, float &t) {
		t = line.vector.normalized().dot(point - line.firstPoint);
		h = line.GetPoint(t);
		//������̓_���炨�낵�������̒�����Ԃ�
		return GetDistancePointAndPoint(h, point);
	}

	//�_�Ɛ����̋���(NG)
	//point, seg	: ���͒l
	//h				: �_���璼���ɑ΂��鐂���̑��̍��W�i�߂�l�j
	//t				: �n�_���琂���̑��ւ̃x�N�g���W���i�߂�l�j
	float GetDistancePointAndSegment(const Vector3f &point, const Segment &seg, Vector3f &h, float &t) {
		//�_�Ɛ����̈ʒu�֌W�𔻒�
		Vector3f p1 = seg.firstPoint;
		Vector3f p2 = seg.GetFinalPoint();
		float result1 = seg.vector.dot(point - p1);
		float result2 = (-seg.vector).dot(point - p2);
		float len = GetDistancePointAndLine(point, seg, h, t);

		//�_�������̎n�_���̊O�ɂ���
		if (result1 <= 0.0f) {
			h = seg.firstPoint;
			return GetDistancePointAndPoint(seg.firstPoint, point);
		}
		//�_�������̏I�_���̊O�ɂ���
		if (result2 <= 0.0f) {
			h = seg.GetFinalPoint();
			return GetDistancePointAndPoint(seg.GetFinalPoint(), point);
		}
		//�_�������̓����ɂ���
		return len;
	}

	//�����ƒ����̋���(NG)
	//line1, line2	: ���͒l
	//pn1, pn2		: �����̑����W�i�߂�l�j
	//t1, t2		: �����̑��ւ̃x�N�g���W���i�߂�l�j
	float GetDistanceLineAndLine(const Line &line1, const Line &line2, Vector3f &pn1, Vector3f &pn2, float &t1, float &t2) {
		//�����x�N�g���Ǝn�_(������̓_)
		Vector3f v1 = line1.vector;
		Vector3f v2 = line2.vector;
		Vector3f p1 = line1.firstPoint;
		Vector3f p2 = line2.firstPoint;

		//���s���H
		if (isParallel(v1, v2)) {
			return GetDistancePointAndPoint(p1, p2);
		}

		//�����̑������߂�
		t1 = (v1.dot(v2) * v2.dot(p1 - p2) - v2.dot(v2) * v1.dot(p1 - p2)) / (v1.dot(v1) * v2.dot(v2) - v1.dot(v2) * v1.dot(v2));
		t2 = v2.dot(p1 + t1 * v1 - p2) / v2.dot(v2);
		pn1 = line1.GetPoint(t1);
		pn2 = line2.GetPoint(t2);

		return GetDistancePointAndPoint(pn1, pn2);
	}

	//�����Ɛ����̋���(NG)
	float GetDistanceSegmentAndSegment(const Segment &seg1, const Segment &seg2) {
		//�������璼���ɂ��낵�������̑�
		Vector3f p1, p2;
		//�����x�N�g��
		Vector3f v1 = seg1.vector;
		Vector3f v2 = seg2.vector;

		//�����̑��̍��W
		Vector3f pn1, pn2;
		//�����̑��ւ̃x�N�g���̌W��
		float t1, t2;
		float len;

		//���s���H
		if (isParallel(v1, v2)) {
			//���s�Ȃ�����̒[��������Е��̐����ɐ��������낵�Đ����̑����������ɂ��邩�𔻒�
			t1 = 0.0f;
			pn1 = seg1.GetPoint(t1);
			len = GetDistancePointAndSegment(pn1, seg2, pn2, t2);
			if (t2 <= 1.0f && t2 >= 0.0f) {
				return len;
			}
		}
		else {
			//���s�łȂ��Ƃ��͒����Ԃ̋��������ɋ��߂Đ����̑����������ɂ��邩�𔻒�
			len = GetDistanceLineAndLine(seg1, seg2, pn1, pn2, t1, t2);
			if (t1 <= 1.0f && t1 >= 0.0f && t2 <= 1.0f && t2 >= 0.0f) {
				return len;
			}
		}
		//���������̊O���Ȃ̂Œl���N�����v���čČv�Z
		Clamp01(t1);
		pn1 = seg1.GetPoint(t1);
		len = GetDistancePointAndLine(pn1, seg2, pn2, t2);
		if (t2 <= 1.0f && t2 >= 0.0f) {
			return len;
		}

		//�܂����������̊O�ɂ���̂ł����Е����N�����v
		Clamp01(t2);
		pn2 = seg2.GetPoint(t2);
		len = GetDistancePointAndLine(pn2, seg1, pn1, t1);
		if (t1 <= 1.0f && t1 >= 0.0f) {
			return len;
		}

		//�������̒[�_�̋������ŒZ
		Clamp01(t1);
		pn1 = seg1.GetPoint(t1);
		len = GetDistancePointAndPoint(pn1, pn2);
		return len;
	}

	//0.0f����1.0f�̊Ԃɒl���N�����v
	void Clamp01(float &v) {
		if (v > 1.0f) {
			v = 1.0f;
			return;
		}
		else if (v < 0.0f) {
			v = 0.0f;
			return;
		}
	}

	//�_�ƕ��ʂ̋���
	float GetDistancePointAndPlane(const Vector3f &point, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3) {
		float result;
		//�ʂ̖@�����Z�o
		Vector3f normal = GetPolygonNormal(p1, p2, p3);
		result = normal.dot(point - p1);
		//ABS��
		result = fabsf(result);
		return result;
	}

	//�O�p�|���S���̖@�����v�Z
	Vector3f GetPolygonNormal(const Vector3f &p1, const Vector3f &p2, const Vector3f &p3) {
		Vector3f v1 = p2 - p1;
		Vector3f v2 = p3 - p2;
		return v1.cross(v2).normalized();
	}
}