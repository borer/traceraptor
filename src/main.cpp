#include <iostream>
#include <string>
#include "Image.h"

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
    		unsigned char r = (unsigned char)(255.f * (float)i/(float)width);
    		unsigned char g = (unsigned char)(255.f * (float)j/(float)height);
    		unsigned char b = (unsigned char)(255.f * 0.2f);
    		image.setPixel(i, j, r, g, b, 255);
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
