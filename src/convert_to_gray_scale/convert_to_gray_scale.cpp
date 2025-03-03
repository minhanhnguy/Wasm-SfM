#include "../stb/stb_image.h"
#include "../stb/stb_image_write.h"
#include <memory>

unsigned char* convert_to_gray_scale(unsigned char* image, int width, int height, int channels) {
    unsigned char* gray_image = new unsigned char[width * height];
    
    for (int i = 0; i < width * height; ++i) {
        int rgb_index = i * channels;
        unsigned char r = image[rgb_index];
        unsigned char g = image[rgb_index + 1];
        unsigned char b = image[rgb_index + 2];
        
        gray_image[i] = static_cast<unsigned char>(0.2989 * r + 0.5870 * g + 0.1140 * b);
    }
    
    return gray_image;
}