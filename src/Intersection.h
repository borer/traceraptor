/*
 * Intersection.h
 *
 *  Created on: May 29, 2017
 *      Author: bogdan
 */

#ifndef SRC_INTERSECTION_H_
#define SRC_INTERSECTION_H_

#include <Vec.h>

namespace traceraptor {

class Shape;
class Material;

struct UV {
	UV(float u, float v) : u(u), v(v) {}

	float u;
	float v;
};

struct IntersectionInfo {
	IntersectionInfo() :
		t(0),
		uv(0,0),
		hit_point(),
		normal(),
        shape(nullptr),
		material(nullptr),
		hit_something(false) {}

    float t;
    UV uv;
    Vec3f hit_point;
    Vec3f normal;
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
    bool hit_something;
};

} /* namespace traceraptor */

#endif /* SRC_INTERSECTION_H_ */
