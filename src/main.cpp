#include <iostream>
#include <string>
#include <random>

#include "Image.h"
#include "Vec3.h"
#include "Ray.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "float.h"

using namespace traceraptor;

void log(const char* message) {
	std::cout << std::endl << message << std::endl;
}

Vec3 color(const Ray& r, Hitable *world) {
	hit_record rec;
	if (world->hit(r, 0.0, MAXFLOAT, rec)) {
		return 0.5*Vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
	}else {
		Vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0-t) * Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
	}
}

float random_float(std::default_random_engine &generator) {
	std::uniform_real_distribution<float> distribution(0.0,1.0);
	return distribution(generator);
}

int main(int argc, char* argv[]) {
	std::string fileName = (argc < 2) ? "output.png" : argv[1];

    const int width = 400;
    const int height =200;
    const int ns = 100;
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
    std::default_random_engine generator;
    log("Beginning ray tracing");
    for (int j = 0; j < height; j++) {
    	for (int i = 0; i < width; i++) {
    		Vec3 col(0,0,0);
    		for (int s=0; s < ns; s++) {
    			float u = ((float)i + random_float(generator))/(float)width;
    			float v = ((float)j + random_float(generator))/(float)height;
    			Ray r = camera.get_ray(u, v);
    			col += color(r, world);
    		}
    		col /= ns;
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
