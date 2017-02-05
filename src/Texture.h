/*
 * Texture.h
 *
 *  Created on: Dec 11, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_TEXTURE_H_
#define TRACERAPTOR_TEXTURE_H_

#include <Vec.h>

#define UNUSED(expr) (void)(expr)

namespace traceraptor {

class Texture {
public:
	virtual ~Texture() {}

	virtual Vec3f value(float u, float v, const Vec3f& p) const = 0;
};

class ConstantTexture : public Texture {
	Vec3f color;

public:
	explicit ConstantTexture(Vec3f c) : color(c) { }
	virtual ~ConstantTexture() {};

	Vec3f value(float u, float v, const Vec3f& p) const {
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

	virtual Vec3f value(float u, float v, const Vec3f& p) const {
		float sines = sin(size_checker*p[0])*sin(size_checker*p[1])*sin(size_checker*p[2]);
		if (sines < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}
};

}

#endif /* TRACERAPTOR_TEXTURE_H_ */
