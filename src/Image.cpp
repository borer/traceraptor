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
	delete[] this->pixels;
}

bool Image::isValidPixelCoordinate(int x, int y)
{
	return (x < width && y < height && x >= 0 && y >= 0);
}

int Image::getPixelIndex(int x, int y) {
	return (x + (height-(y+1))*width)*CHANNELS_PER_PIXEL;
}

void Image::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	if (!isValidPixelCoordinate(x,y))
	{
		Logger::log_debug("Discarded pixel : [" + std::to_string(x) + ", " + std::to_string(y) + "]");
		return;
	}

	int index = getPixelIndex(x, y);
	this->pixels[index] = r;
	this->pixels[++index] = g;
	this->pixels[++index] = b;
	this->pixels[++index] = a;
}

void Image::setPixel(int x, int y, const Vec3 &color){
	if (!isValidPixelCoordinate(x,y))
	{
		Logger::log_debug("Discarded pixel : [" + std::to_string(x) + ", " + std::to_string(y) + "]");
		return;
	}

	int index = getPixelIndex(x, y);
	this->pixels[index] = (unsigned char)(255.99f * color.r());
	this->pixels[++index] = (unsigned char)(255.99f * color.g());
	this->pixels[++index] = (unsigned char)(255.99f * color.b());
	this->pixels[++index] = 255;
}

int Image::writePNGfile(std::string filename) {
	int stride_in_bytes = width*CHANNELS_PER_PIXEL;
	int result = stbi_write_png(filename.c_str(), width, height, CHANNELS_PER_PIXEL, pixels, stride_in_bytes);
	return result;
}

} /* namespace traceraptor */
