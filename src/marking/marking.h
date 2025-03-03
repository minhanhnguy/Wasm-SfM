#ifndef MARKING_H
#define MARKING_H
#include "../stb/stb_image.h"
#include "../stb/stb_image_write.h"
#include <vector>

unsigned char* mark_image(unsigned char* image, int width, int height, int channels, std::vector<std::vector<int>> keypoints);
unsigned char* mark_gray_image_rgb(unsigned char* image, int width, int height, std::vector<std::vector<int>> keypoints);

#endif