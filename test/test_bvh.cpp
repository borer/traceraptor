#define CATCH_CONFIG_MAIN
#include "catch.h"

#include "Logger.h"
#include "Vec3.h"
#include "Hitable.h"
#include "Sphere.h"
#include "Renderer.h"
#include "BVH.h"
#include "Texture.h"

using namespace traceraptor;

bool test_simple_bvh() {

	std::vector<std::shared_ptr<Hitable>> list(6);
	list[0] = std::shared_ptr<Hitable>(new Sphere(Vec3(0,0,0),
			0.5,
			std::shared_ptr<Material>(new Lambertian(std::make_shared<ConstantTexture>(Vec3(0.8, 0.3, 0.3))))));
	list[1] = std::shared_ptr<Hitable>(new Sphere(Vec3(0,-1000.5,0),
			1000,
			std::shared_ptr<Material>(new Lambertian(std::make_shared<ConstantTexture>(Vec3(0.5, 0.5, 0.5))))));
	list[2] = std::shared_ptr<Hitable>(new Sphere(Vec3(1,0,0),
			0.5,
			std::shared_ptr<Material>(new Metal(Vec3(0.7, 0.6, 0.5), 0.2))));
	list[3] = std::shared_ptr<Hitable>(new Sphere(Vec3(-1,0,0),
			0.5,
			std::shared_ptr<Material>(new Dielectric(1.2))));
	list[4] = std::shared_ptr<Hitable>(new Sphere(Vec3(-1,0,0),
			-0.45,
			std::shared_ptr<Material>(new Dielectric(1.2))));
	list[5] = std::shared_ptr<Hitable>(new Sphere(Vec3(1,0,-1),
			0.5,
			std::shared_ptr<Material>(new Lambertian(std::make_shared<ConstantTexture>(Vec3(0.4, 0.2, 0.1))))));

	BVH world(list, 3);

	return true;
}

TEST_CASE( "BVH", "[BVH]" ) {
	 REQUIRE( test_simple_bvh() );
}
