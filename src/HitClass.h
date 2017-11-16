#pragma once

#include"HitPrimitive.h"

//あたり判定
namespace Hit {
	//計算
	//0～1の範囲にfloatの値を収めて返す
	void  Clamp01(float &v);
	//平面と点の距離
	float GetDistancePointAndPlane(const Vector3f &point, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3);
	//ポリゴンの法線を取得
	Vector3f GetPolygonNormal(const Vector3f &point1, const Vector3f &point2, const Vector3f &point3);
	//二点間の距離を算出
	float GetDistancePointAndPoint(const Vector3f &p1, const Vector3f &p2);
	//点と直線の距離
	float GetDistancePointAndLine(const Vector3f &p, const Line &l, Vector3f &h, float &t);
	//点と線分の距離
	float GetDistancePointAndSegment(const Vector3f &p, const Segment &s, Vector3f &h, float &t);
	//直線と直線の距離、垂線の足の座標と始点からのベクトル係数を返す
	float GetDistanceLineAndLine(const Line &line1, const Line &line2, Vector3f &pn1, Vector3f &pn2, float &t1, float &t2);
	//線分と線分の距離
	float GetDistanceSegmentAndSegment(const Segment &seg1, const Segment &seg2);
	//線分とポリゴンの距離
	float GetDistanceSegmentAndPolygon(const Segment &seg, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3);

	//判定
	//ポリゴンとカプセル
	bool IsHitPolygonAndCapsule(const Capsule &capsule, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, Vector3f &crossPoint);
	//ポリゴンと線分の衝突
	bool HitSegmentAndPolygon(const Segment &segment, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, Vector3f &crossPoint);
	//カプセルとカプセル
	bool HitCapsuleAndCapsule(const Capsule &c1, const Capsule &c2);
	//ポリゴン内に点が含まれるか
	bool isPointinPolygon(const Vector3f &p, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3);
	//線分と平面の衝突
	bool isHitSegmentAndPlane(const Segment &segment, const Vector3f &polygon, const Vector3f &p2, const Vector3f &p3);
}