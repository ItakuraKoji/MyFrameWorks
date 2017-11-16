#pragma once
#include<Eigen\Core>
#include<Eigen\Geometry>
using namespace Eigen;

//誤差定数と、誤差内なら同じとみなすマクロ(bool)
#define EPSILON 0.000001f
#define EPSILON_equal(d, comp) (-EPSILON + (comp) < (d) && (d) < EPSILON + (comp))

//あたり判定のためのコリジョンの構造体定義
namespace Hit {

	//2ベクトルの関係
	bool isVertical(const Vector3f &vector, const Vector3f &vector2);
	bool isParallel(const Vector3f &vector, const Vector3f &vector2);
	bool isSharp(const Vector3f &vector, const Vector3f &vector2);

	//線は始点と、そこから延びる方向ベクトルで定義
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

	//線分は直線に終点を持たせたもの
	struct Segment : public Line {
	public:
		Segment();
		Segment(const Vector3f &p, const Vector3f &v);
		//Segment(const Point &fp, const Point &finalPoint);
		Vector3f GetFinalPoint() const;
	};

	//球は中心点と半径で表す
	struct Sphere {
	public:
		Sphere();
		Sphere(const Vector3f &p, const float r);
	public:
		Vector3f center;
		float radius;
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

	//カプセルは線分と半径
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