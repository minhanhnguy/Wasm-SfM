#ifndef CONVERT_TO_GRAY_SCALE_H
#define CONVERT_TO_GRAY_SCALE_H
#include "../stb/stb_image.h"
#include "../stb/stb_image_write.h"

unsigned char* convert_to_gray_scale(unsigned char* image, int width, int height, int channels);

#endif