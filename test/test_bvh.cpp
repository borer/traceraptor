
#define CATCH_CONFIG_MAIN
#include "catch.h"

#include "Logger.h"
#include <Vec.h>
#include "Hitable.h"
#include "Sphere.h"
#include "Renderer.h"
#include "BVH.h"
#include "Texture.h"

using namespace traceraptor;

bool test_simple_bvh() {

	std::vector<std::shared_ptr<Hitable>> list(6);
	list[0] = std::shared_ptr<Hitable>(new Sphere(Vec3f{0.f,0.f,0.f},
			0.5,
			std::shared_ptr<Material>(new Lambertian(std::make_shared<ConstantTexture>(Vec3f{0.8f, 0.3f, 0.3f})))));
	list[1] = std::shared_ptr<Hitable>(new Sphere(Vec3f{0.f,-1000.5f,0.f},
			1000,
			std::shared_ptr<Material>(new Lambertian(std::make_shared<ConstantTexture>(Vec3f{0.5, 0.5, 0.5})))));
	list[2] = std::shared_ptr<Hitable>(new Sphere(Vec3f{1.f,0.f,0.f},
			0.5,
			std::shared_ptr<Material>(new Metal(Vec3f{0.7f, 0.6f, 0.5f}, 0.2))));
	list[3] = std::shared_ptr<Hitable>(new Sphere(Vec3f{-1.f,0.f,0.f},
			0.5,
			std::shared_ptr<Material>(new Dielectric(1.2))));
	list[4] = std::shared_ptr<Hitable>(new Sphere(Vec3f{-1,0,0},
			-0.45,
			std::shared_ptr<Material>(new Dielectric(1.2))));
	list[5] = std::shared_ptr<Hitable>(new Sphere(Vec3f{1,0,-1},
			0.5,
			std::shared_ptr<Material>(new Lambertian(std::make_shared<ConstantTexture>(Vec3f{0.4, 0.2, 0.1})))));

	BVH world(list, 3);

	return true;
}

TEST_CASE( "BVH", "[BVH]" ) {
	 REQUIRE( test_simple_bvh() );
}
