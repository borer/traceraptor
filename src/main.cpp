#include <iostream>
#include <string>
#include "Image.h"
#include "Vec3.h"
#include "Ray.h"

using namespace traceraptor;

void log(const char* message) {
	std::cout << std::endl << message << std::endl;
}

Vec3 color(const Ray& r) {
	Vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5f*(unit_direction.y() + 1.0);
	return (1.0-t) * Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
}

int main(int argc, char* argv[]) {
	std::string fileName = (argc < 2) ? "output.png" : argv[1];

    const int width = 200;
    const int height = 100;
    Image image(width, height);

    Vec3 lower_left_corner(-2.0, -1.0, -1.0);
    Vec3 horizontal(4.0, 0.0, 0.0);
    Vec3 vertical(0.0, 2.0, 0.0);
    Vec3 origin(0.0, 0.0, 0.0);

    log("Beginning ray tracing");
    for (int j = 0; j < height; j++) {
    	for (int i = 0; i < width; i++) {
    		float u = (float)i/(float)width;
    		float v = (float)j/(float)height;
    		Ray r(origin, lower_left_corner + u*horizontal + v*vertical);
    		Vec3 col = color(r);
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
