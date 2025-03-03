#include "../stb/stb_image.h"
#include "../stb/stb_image_write.h"
#include <vector>

const float alpha = 0.5f;
const int size = 2; 

unsigned char* mark_image(unsigned char* image, int width, int height, int channels, std::vector<std::vector<int>> keypoints) {
    for (const auto& keypoint : keypoints) {
        int x = keypoint[0];
        int y = keypoint[1];
        

        int color[3] = {255, 255, 255};

        for (int i = -size; i <= size; ++i) {
            int nx1 = x + i;
            int ny1 = y + i;
            if (nx1 >= 0 && nx1 < width && ny1 >= 0 && ny1 < height) {
                int index = (ny1 * width + nx1) * channels;
                
                if (channels == 4) {
                    unsigned char* pixel = &image[index];
                    pixel[0] = static_cast<unsigned char>(alpha * color[0] + (1 - alpha) * pixel[0]); 
                    pixel[1] = static_cast<unsigned char>(alpha * color[1] + (1 - alpha) * pixel[1]); 
                    pixel[2] = static_cast<unsigned char>(alpha * color[2] + (1 - alpha) * pixel[2]); 
                    pixel[3] = 255; 
                } else {
                    image[index] = static_cast<unsigned char>(alpha * color[0] + (1 - alpha) * image[index]); 
                    image[index + 1] = static_cast<unsigned char>(alpha * color[1] + (1 - alpha) * image[index + 1]); 
                    image[index + 2] = static_cast<unsigned char>(alpha * color[2] + (1 - alpha) * image[index + 2]); 
                }
            }
        }

        for (int i = -size; i <= size; ++i) {
            int nx2 = x - i;
            int ny2 = y + i;
            if (nx2 >= 0 && nx2 < width && ny2 >= 0 && ny2 < height) {
                int index = (ny2 * width + nx2) * channels;
                
                if (channels == 4) {
                    unsigned char* pixel = &image[index];
                    pixel[0] = static_cast<unsigned char>(alpha * color[0] + (1 - alpha) * pixel[0]); 
                    pixel[1] = static_cast<unsigned char>(alpha * color[1] + (1 - alpha) * pixel[1]); 
                    pixel[2] = static_cast<unsigned char>(alpha * color[2] + (1 - alpha) * pixel[2]); 
                    pixel[3] = 255; 
                } else {
                    image[index] = static_cast<unsigned char>(alpha * color[0] + (1 - alpha) * image[index]); 
                    image[index + 1] = static_cast<unsigned char>(alpha * color[1] + (1 - alpha) * image[index + 1]);
                    image[index + 2] = static_cast<unsigned char>(alpha * color[2] + (1 - alpha) * image[index + 2]); 
                }
            }
        }
    }

    return image;
}

unsigned char* mark_gray_image_rgb(unsigned char* gray_image, int width, int height, std::vector<std::vector<int>> keypoints) {
    unsigned char marker_color = 255;
    
    unsigned char* rgb_image = new unsigned char[width * height * 3];
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int gray_idx = y * width + x;
            int rgb_idx = (y * width + x) * 3;
            
            rgb_image[rgb_idx] = gray_image[gray_idx];
            rgb_image[rgb_idx + 1] = gray_image[gray_idx];
            rgb_image[rgb_idx + 2] = gray_image[gray_idx];
        }
    }
    
    for (const auto& keypoint : keypoints) {
        int x = keypoint[0];
        int y = keypoint[1];

        for (int i = -size; i <= size; ++i) {
            int nx1 = x + i;
            int ny1 = y + i;
            if (nx1 >= 0 && nx1 < width && ny1 >= 0 && ny1 < height) {
                int rgb_idx = (ny1 * width + nx1) * 3;
                for (int c = 0; c < 3; ++c) {
                    rgb_image[rgb_idx + c] = static_cast<unsigned char>(
                        alpha * marker_color + (1 - alpha) * rgb_image[rgb_idx + c]
                    );
                }
            }

            int nx2 = x - i;
            int ny2 = y + i;
            if (nx2 >= 0 && nx2 < width && ny2 >= 0 && ny2 < height) {
                int rgb_idx = (ny2 * width + nx2) * 3;
                for (int c = 0; c < 3; ++c) {
                    rgb_image[rgb_idx + c] = static_cast<unsigned char>(
                        alpha * marker_color + (1 - alpha) * rgb_image[rgb_idx + c]
                    );
                }
            }
        }
    }
    return rgb_image;
}