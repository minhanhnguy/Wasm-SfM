#include "./convert_to_gray_scale/convert_to_gray_scale.h"
#include "./stb/stb_image_write.h"
#include "./marking/marking.h"
#include "./stb/stb_image.h"
#include "./orb/orb.h"

#include <iostream>
#include <cstring>
#include <string>
#include <vector>

int main() {
    int width, height, channels;
    for(int index=1;index<=6;index++){
        std::cout << "Processing image " << index << std::endl;

        // Input images
        std::string input = "./!input/" + std::to_string(index) + ".jpg";
        unsigned char* image = stbi_load(input.c_str(), &width, &height, &channels, 0); 
        unsigned char* image_copy = new unsigned char[width * height * channels];
        std::memcpy(image_copy, image, width * height * channels);

        // Convert to grayscale
        unsigned char* gray_scale_image = convert_to_gray_scale(image_copy, width, height, channels);
        std::cout << "Converted to grayscale" << std::endl;
        
        // Detect keypoints
        std::vector<std::vector<int>> keypoints = detect_keypoints(gray_scale_image, width, height);
        std::cout << "Detected keypoints" << std::endl;
        
        // Mark keypoints on grayscale image
        unsigned char* marked_gray_image = mark_gray_image_rgb(gray_scale_image, width, height, keypoints);
        std::string output_gray_image = "./!output_gray/" + std::to_string(index) + ".jpg"; 
        stbi_write_jpg(output_gray_image.c_str(), width, height, channels, marked_gray_image, 100);
        std::cout << "Saved grayscale image " << output_gray_image << std::endl;

        // Mark keypoints
        unsigned char* marked_image = mark_image(image, width, height, channels, keypoints);
        std::string output = "./!output/" + std::to_string(index) + ".jpg"; 
        stbi_write_jpg(output.c_str(), width, height, channels, marked_image, 100);
        std::cout << "Saved marked image " << output << std::endl;

        stbi_image_free(image);
        delete[] gray_scale_image;

        std::cout << index << ". Image processed and saved." << std::endl;
    }
    return 0;
}
