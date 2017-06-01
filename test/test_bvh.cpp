
#define CATCH_CONFIG_MAIN
#include "catch.h"

#include "Logger.h"
#include <Vec.h>
#include <Shape.h>
#include "shapes/Sphere.h"
#include "Renderer.h"
#include "BVH.h"
#include "Texture.h"

using namespace traceraptor;

bool test_simple_bvh() {

	return true;
}

TEST_CASE( "BVH", "[BVH]" ) {
	 REQUIRE( test_simple_bvh() );
}
