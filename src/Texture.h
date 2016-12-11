/*
 * Texture.h
 *
 *  Created on: Dec 11, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_TEXTURE_H_
#define TRACERAPTOR_TEXTURE_H_

#include "Vec3.h"

#define UNUSED(expr) (void)(expr)

namespace traceraptor {

class Texture {
public:
	virtual ~Texture() {}

	virtual Vec3 value(float u, float v, const Vec3& p) const = 0;
};

class ConstantTexture : public Texture {
	Vec3 color;

public:
	explicit ConstantTexture(Vec3 c) : color(c) { }
	virtual ~ConstantTexture() {};

	Vec3 value(float u, float v, const Vec3& p) const {
		UNUSED(u);UNUSED(v);UNUSED(p);
		return color;
	}
};

class CheckerTexture : public Texture {
	std::shared_ptr<Texture> even;
	std::shared_ptr<Texture> odd;
	float size_checker;
public:
	CheckerTexture(std::shared_ptr<Texture> t0, std::shared_ptr<Texture> t1, float size_checker):
		even(t0), odd(t1), size_checker(size_checker) { }
	virtual ~CheckerTexture() {};

	virtual Vec3 value(float u, float v, const Vec3& p) const {
		float sines = sin(size_checker*p.x())*sin(size_checker*p.y())*sin(size_checker*p.z());
		if (sines < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}
};

}

#endif /* TRACERAPTOR_TEXTURE_H_ */
