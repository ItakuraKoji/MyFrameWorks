#pragma once
#include<Eigen\Core>
#include<Eigen\Geometry>
using namespace Eigen;

//�덷�萔�ƁA�덷���Ȃ瓯���Ƃ݂Ȃ��}�N��(bool)
#define EPSILON 0.000001f
#define EPSILON_equal(d, comp) (-EPSILON + (comp) < (d) && (d) < EPSILON + (comp))

//�����蔻��̂��߂̃R���W�����̍\���̒�`
namespace Hit {

	//2�x�N�g���̊֌W
	bool isVertical(const Vector3f &vector, const Vector3f &vector2);
	bool isParallel(const Vector3f &vector, const Vector3f &vector2);
	bool isSharp(const Vector3f &vector, const Vector3f &vector2);

	//���͎n�_�ƁA�������牄�т�����x�N�g���Œ�`
	struct Line {
	public:
		Line();
		Line(const Vector3f &p, const Vector3f &v);
		Vector3f GetPoint(const float t) const;
	public:
		Vector3f firstPoint;
		Vector3f vector;
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

	//�����͒����ɏI�_��������������
	struct Segment : public Line {
	public:
		Segment();
		Segment(const Vector3f &p, const Vector3f &v);
		//Segment(const Point &fp, const Point &finalPoint);
		Vector3f GetFinalPoint() const;
	};

	//���͒��S�_�Ɣ��a�ŕ\��
	struct Sphere {
	public:
		Sphere();
		Sphere(const Vector3f &p, const float r);
	public:
		Vector3f center;
		float radius;
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

	//�J�v�Z���͐����Ɣ��a
	struct Capsule {
	public:
		Capsule();
		Capsule(const Segment &s, const float r);
		Capsule(const Vector3f &p1, const Vector3f &p2, const float r);
	public:
		Segment segment;
		float radius;
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};


}