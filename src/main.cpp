#include <iostream>
#include <string>

#include "Image.h"
#include "Vec3.h"
#include "Ray.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "UtilMath.h"

using namespace traceraptor;

void log(const char* message) {
	std::cout << std::endl << message << std::endl;
}

void log(std::string message) {
	std::cout << std::endl << message << std::endl;
}

Vec3 color(const Ray& r, Hitable *world, int current_ray_bounce, int MAX_RAY_BOUNCE) {
	hit_record rec;
	if (world->hit(r, 0.0, MAXFLOAT, rec)) {
		if (current_ray_bounce < MAX_RAY_BOUNCE) {
			Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
			return 0.5*color(Ray(rec.p, target-rec.p), world, current_ray_bounce+1, MAX_RAY_BOUNCE);
		} else {
			return Vec3(0, 0, 0);
		}
	} else {
		Vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return lerp(Vec3(1.0, 1.0, 1.0), Vec3(0.5, 0.7, 1.0), t);
	}
}

int main(int argc, char* argv[]) {
	std::string fileName = (argc < 2) ? "output.png" : argv[1];

    const int width = 200;
    const int height = 100;
    const int ns = 20;
    const int MAX_RAY_BOUNCE = 50;
    Image image(width, height);

    Vec3 lower_left_corner(-2.0, -1.0, -1.0);
    Vec3 horizontal(4.0, 0.0, 0.0);
    Vec3 vertical(0.0, 2.0, 0.0);
    Vec3 origin(0.0, 0.0, 0.0);

    Hitable *list[2];
    list[0] = new Sphere(Vec3(0,0,-1), 0.5);
    list[1] = new Sphere(Vec3(0,-100.5,-1), 100);
    Hitable *world = new HitableList(list, 2);

    Camera camera;
    log("Beginning ray tracing");
    for (int j = 0; j < height; j++) {
    	for (int i = 0; i < width; i++) {
    		log("Iteration [" + std::to_string(i) + ", " + std::to_string(j) +"] out of [" + std::to_string(width) + ", " + std::to_string(height) + "]");
    		Vec3 col(0,0,0);
    		for (int s=0; s < ns; s++) {
    			float u = ((float)i + random01())/(float)width;
    			float v = ((float)j + random01())/(float)height;
    			Ray r = camera.get_ray(u, v);
    			col += color(r, world, 0, MAX_RAY_BOUNCE);
    		}
    		col /= ns;
    		col = Vec3(std::sqrt(col[0]), std::sqrt(col[1]), std::sqrt(col[2]));
    		image.setPixel(i, j, col);
    	}
    }

    log("Beginning image writing");
    int result = image.writePNGfile(fileName);
    if (result < 0) {
    	log("cannot create image file");
    	return -1;
    }

    return 0;
}
