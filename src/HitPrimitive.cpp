#include"HitPrimitive.h"

namespace Hit {
	//‚’¼‚©H
	bool isVertical(const Vector3f &vector, const Vector3f &vector2) {
		return EPSILON_equal(vector.dot(vector2), 0.0f);
	}
	//•½s‚©H
	bool isParallel(const Vector3f &vector, const Vector3f &vector2) {
		return EPSILON_equal(vector.cross(vector2).norm(), 0.0f);
	}
	//‰sŠp‚©H
	bool isSharp(const Vector3f &vector, const Vector3f &vector2) {
		return vector.dot(vector2) > 0.0f;
	}

	Line::Line() : firstPoint(0.0f, 0.0f, 0.0f), vector(0.0f, 0.0f, 0.0f) {}
	Line::Line(const Vector3f &p, const Vector3f &v) : firstPoint(p), vector(v) {}
	Vector3f Line::GetPoint(const float t) const {
		return firstPoint + t * vector;
	}

	Segment::Segment() {}
	Segment::Segment(const Vector3f &p, const Vector3f &v) : Line(p, v) {}
	//Segment::Segment(const Point &fp, const Point &finalPoint) : Line(fp, (Vector3f)(finalPoint - fp)) {}
	Vector3f Segment::GetFinalPoint() const {
		return firstPoint + vector;
	}

	Sphere::Sphere() : center(0.0f, 0.0f, 0.0f), radius(1.0f) {}
	Sphere::Sphere(const Vector3f &p, const float r) : center(p), radius(r) {}

	Capsule::Capsule() {}
	Capsule::Capsule(const Segment &s, const float r) : segment(s), radius(r) {}
	Capsule::Capsule(const Vector3f &p1, const Vector3f &p2, const float r) : segment(p1, p2), radius(r) {}
}