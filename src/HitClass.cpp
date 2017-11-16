#include"HitClass.h"

namespace Hit {
	//線分とポリゴンの距離(NG)
	float GetDistanceSegmentAndPolygon(const Segment &seg, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3) {
		//線分とポリゴンが交差している場合の距離は０
		Vector3f crossPoint;
		if (HitSegmentAndPolygon(seg, p1, p2, p3, crossPoint)) {
			return 0.0f;
		}

		float returnLength = 0.0f;
		Vector3f verticalFoot;
		Vector3f firstPoint = seg.firstPoint;
		Vector3f finalPoint = seg.GetFinalPoint();

		//両端からポリゴンを含む平面の距離を取り、垂線の足がポリゴン内ならその距離をかえす
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

		//さらにポリゴンの三点を取ってより短い距離を採用
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
	//ポリゴンとカプセルの衝突(NG)
	//capsule : 判定をとるカプセル
	//p1, p2, p3 : ポリゴンの3点
	//crossPoint : 衝突点
	bool IsHitPolygonAndCapsule(const Capsule &capsule, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, Vector3f &crossPoint) {
		float length = GetDistanceSegmentAndPolygon(capsule.segment, p1, p2, p3);
		if (length > capsule.radius) {
			return false;
		}

		return true;
	}



	//ポリゴンと線分の衝突
	//segment, p1, p2, p3: 入力
	//crossPoint: 押し出し座標（戻り値）
	bool HitSegmentAndPolygon(const Segment &segment, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, Vector3f &crossPoint) {
		//ポリゴンを含む平面と線分が衝突しているかを調べる
		if (!isHitSegmentAndPlane(segment, p1, p2, p3)) {
			return false;
		}

		//線分とポリゴンの内分比
		float s;
		//平面と線分の交点座標
		Vector3f point;
		//始点と終点から平面への距離
		float d1, d2;

		d1 = GetDistancePointAndPlane(segment.firstPoint, p1, p2, p3);
		d2 = GetDistancePointAndPlane(segment.GetFinalPoint(), p1, p2, p3);

		//線分と平面の交点の内分比sを求める
		//（始点から交点：交点から終点　＝　d1：d2　＝　s：(1-s) ）
		if (d1 + d2) {
			s = d1 / (d1 + d2);
		}
		else {
			s = 0;
		}

		//内分比から座標を算出
		point = segment.firstPoint + segment.vector * s;

		//算出した点がポリゴン内に含まれるかを調べる
		if (!isPointinPolygon(point, p1, p2, p3)) {
			return false;
		}

		//衝突地点から少し戻った地点をあたり判定の押し戻し値として返す
		crossPoint = segment.firstPoint + segment.vector * (s - 0.01f);
		return true;
	}

	//カプセルとカプセルの衝突(NG?)
	//c1, c2	: 入力
	bool HitCapsuleAndCapsule(const Capsule &c1, const Capsule &c2) {
		float len = GetDistanceSegmentAndSegment(c1.segment, c2.segment);
		float radius = c1.radius + c2.radius;
		if (len > radius) {
			return false;
		}
		return true;
	}

	//平面と線分の衝突
	//segment		: 入力値
	//p1, p2, p3	: 平面上の三点
	bool isHitSegmentAndPlane(const Segment &segment, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3) {
		//面の法線を算出
		Vector3f normal = GetPolygonNormal(p1, p2, p3);

		//線分が平面に含まれているか(法線と線分が垂直か)どうかを調べる
		if (normal.dot(segment.vector) == 0.0f && normal.dot(p1 - segment.firstPoint) != 0.0f) {
			return false;
		}

		//線分が交差しているかを判定
		float result = normal.dot(segment.firstPoint - p1) * normal.dot(segment.GetFinalPoint() - p1);
		if (result > 0) {
			return false;
		}
		return true;
	}




	//同平面上の三角ポリゴン内に点が含まれるかどうか
	//p				: 任意の点
	//p1, p2, p3	: ポリゴンの三点（反時計回りが表）
	bool isPointinPolygon(const Vector3f &p, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3) {
		//辺ベクトル
		Vector3f v1 = p2 - p1;
		Vector3f v2 = p3 - p2;
		Vector3f v3 = p1 - p3;
		Vector3f normal = GetPolygonNormal(p1, p2, p3);
		Vector3f result;

		//法線と外積で得られたベクトルがすべて同じなら点がポリゴン内にある
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

	//二点の距離
	float GetDistancePointAndPoint(const Vector3f &p1, const Vector3f &p2) {
		return (p2 - p1).norm();
	}

	//点と直線の距離(NG)
	//point, line	: 入力値
	//h				: 点から直線に対する垂線の足の座標（戻り値）
	//t				: 始点から垂線の足へのベクトル係数（戻り値）
	float GetDistancePointAndLine(const Vector3f &point, const Line &line, Vector3f &h, float &t) {
		t = line.vector.normalized().dot(point - line.firstPoint);
		h = line.GetPoint(t);
		//直線上の点からおろした垂線の長さを返す
		return GetDistancePointAndPoint(h, point);
	}

	//点と線分の距離(NG)
	//point, seg	: 入力値
	//h				: 点から直線に対する垂線の足の座標（戻り値）
	//t				: 始点から垂線の足へのベクトル係数（戻り値）
	float GetDistancePointAndSegment(const Vector3f &point, const Segment &seg, Vector3f &h, float &t) {
		//点と線分の位置関係を判定
		Vector3f p1 = seg.firstPoint;
		Vector3f p2 = seg.GetFinalPoint();
		float result1 = seg.vector.dot(point - p1);
		float result2 = (-seg.vector).dot(point - p2);
		float len = GetDistancePointAndLine(point, seg, h, t);

		//点が線分の始点側の外にいる
		if (result1 <= 0.0f) {
			h = seg.firstPoint;
			return GetDistancePointAndPoint(seg.firstPoint, point);
		}
		//点が線分の終点側の外にいる
		if (result2 <= 0.0f) {
			h = seg.GetFinalPoint();
			return GetDistancePointAndPoint(seg.GetFinalPoint(), point);
		}
		//点が線分の内側にある
		return len;
	}

	//直線と直線の距離(NG)
	//line1, line2	: 入力値
	//pn1, pn2		: 垂線の足座標（戻り値）
	//t1, t2		: 垂線の足へのベクトル係数（戻り値）
	float GetDistanceLineAndLine(const Line &line1, const Line &line2, Vector3f &pn1, Vector3f &pn2, float &t1, float &t2) {
		//直線ベクトルと始点(直線上の点)
		Vector3f v1 = line1.vector;
		Vector3f v2 = line2.vector;
		Vector3f p1 = line1.firstPoint;
		Vector3f p2 = line2.firstPoint;

		//平行か？
		if (isParallel(v1, v2)) {
			return GetDistancePointAndPoint(p1, p2);
		}

		//垂線の足を求める
		t1 = (v1.dot(v2) * v2.dot(p1 - p2) - v2.dot(v2) * v1.dot(p1 - p2)) / (v1.dot(v1) * v2.dot(v2) - v1.dot(v2) * v1.dot(v2));
		t2 = v2.dot(p1 + t1 * v1 - p2) / v2.dot(v2);
		pn1 = line1.GetPoint(t1);
		pn2 = line2.GetPoint(t2);

		return GetDistancePointAndPoint(pn1, pn2);
	}

	//線分と線分の距離(NG)
	float GetDistanceSegmentAndSegment(const Segment &seg1, const Segment &seg2) {
		//直線から直線におろした垂線の足
		Vector3f p1, p2;
		//直線ベクトル
		Vector3f v1 = seg1.vector;
		Vector3f v2 = seg2.vector;

		//垂線の足の座標
		Vector3f pn1, pn2;
		//垂線の足へのベクトルの係数
		float t1, t2;
		float len;

		//平行か？
		if (isParallel(v1, v2)) {
			//平行なら線分の端からもう片方の線分に垂線をおろして垂線の足が線分内にあるかを判定
			t1 = 0.0f;
			pn1 = seg1.GetPoint(t1);
			len = GetDistancePointAndSegment(pn1, seg2, pn2, t2);
			if (t2 <= 1.0f && t2 >= 0.0f) {
				return len;
			}
		}
		else {
			//平行でないときは直線間の距離を仮に求めて垂線の足が線分内にあるかを判定
			len = GetDistanceLineAndLine(seg1, seg2, pn1, pn2, t1, t2);
			if (t1 <= 1.0f && t1 >= 0.0f && t2 <= 1.0f && t2 >= 0.0f) {
				return len;
			}
		}
		//足が線分の外側なので値をクランプして再計算
		Clamp01(t1);
		pn1 = seg1.GetPoint(t1);
		len = GetDistancePointAndLine(pn1, seg2, pn2, t2);
		if (t2 <= 1.0f && t2 >= 0.0f) {
			return len;
		}

		//まだ足が線分の外にあるのでもう片方もクランプ
		Clamp01(t2);
		pn2 = seg2.GetPoint(t2);
		len = GetDistancePointAndLine(pn2, seg1, pn1, t1);
		if (t1 <= 1.0f && t1 >= 0.0f) {
			return len;
		}

		//両線分の端点の距離が最短
		Clamp01(t1);
		pn1 = seg1.GetPoint(t1);
		len = GetDistancePointAndPoint(pn1, pn2);
		return len;
	}

	//0.0fから1.0fの間に値をクランプ
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

	//点と平面の距離
	float GetDistancePointAndPlane(const Vector3f &point, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3) {
		float result;
		//面の法線を算出
		Vector3f normal = GetPolygonNormal(p1, p2, p3);
		result = normal.dot(point - p1);
		//ABSね
		result = fabsf(result);
		return result;
	}

	//三角ポリゴンの法線を計算
	Vector3f GetPolygonNormal(const Vector3f &p1, const Vector3f &p2, const Vector3f &p3) {
		Vector3f v1 = p2 - p1;
		Vector3f v2 = p3 - p2;
		return v1.cross(v2).normalized();
	}
}