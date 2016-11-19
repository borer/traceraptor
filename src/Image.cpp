/*
 * Image.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: bogdan
 */

#include "Image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace traceraptor {

Image::Image(int width, int height) {
	this->width = width;
	this->height = height;
	this->pixels = new unsigned char[width*height*CHANNELS_PER_PIXEL] {0};
}

Image::~Image() {
	delete this->pixels;
}

bool Image::isValidPixelCoordinate(int x, int y)
{
	return (x <= (width-CHANNELS_PER_PIXEL) && y <= (height-CHANNELS_PER_PIXEL) && x >= 0 && y >= 0);
}

void Image::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	if (!isValidPixelCoordinate(x,y))
	{
		return;
	}

	int index = (x + y*width)*CHANNELS_PER_PIXEL;
	this->pixels[index] = r;
	this->pixels[++index] = g;
	this->pixels[++index] = b;
	this->pixels[++index] = a;
}

int Image::writePNGfile(std::string filename) {
	int stride_in_bytes = width*CHANNELS_PER_PIXEL;
	int result = stbi_write_png(filename.c_str(), width, height, CHANNELS_PER_PIXEL, pixels, stride_in_bytes);
	return result;
}

} /* namespace traceraptor */
