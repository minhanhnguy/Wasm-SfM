#ifndef ORB_H
#define ORB_H
#include "../stb/stb_image.h"
#include "../stb/stb_image_write.h"
#include <cmath>
#include <vector>

std::vector<std::vector<int>> detect_keypoints(unsigned char* image, int width, int height);

#endif