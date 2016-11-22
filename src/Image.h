/*
 * Image.h
 *
 *  Created on: Nov 19, 2016
 *      Author: bogdan
 */

#ifndef SRC_IMAGE_H_
#define SRC_IMAGE_H_

#include <string>
#include "Vec3.h"

namespace traceraptor {

class Image {
	unsigned char* pixels;
	int width;
	int height;
	const int CHANNELS_PER_PIXEL = 4;

	bool isValidPixelCoordinate(int x, int y);
	int getPixelIndex(int x, int y);

public:
	Image(int width, int height);
	virtual ~Image();

	void setPixel(int x, int y, const Vec3 color);
	void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	int writePNGfile(std::string filename);
};

} /* namespace traceraptor */

#endif /* SRC_IMAGE_H_ */
