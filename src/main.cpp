#include <iostream>
#include <string>
#include "Image.h"
#include "Vec3.h"

using namespace traceraptor;

void log(const char* message) {
	std::cout << std::endl << message << std::endl;
}

int main(int argc, char* argv[]) {
	std::string fileName = (argc < 2) ? "output.png" : argv[1];

    const int width = 200;
    const int height = 100;
    Image image(width, height);

    log("Beginning ray tracing");
    for (int j = 0; j < height; j++) {
    	for (int i = 0; i < width; i++) {
    		Vec3 color((float)i/(float)width, (float)j/(float)height, 0.2f);
    		image.setPixel(i, j, color);
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
