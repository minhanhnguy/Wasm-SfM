#include <cstdint>
#include <cstdlib>

extern "C" {
    void convert_to_gray(std::uint8_t* image, int width, int height) {
        for (int i = 0; i < width * height * 4; i += 4) {
            uint8_t r = image[i];
            uint8_t g = image[i + 1];
            uint8_t b = image[i + 2];

            uint8_t gray = (r + g + b) / 3;

            image[i] = gray;
            image[i + 1] = gray;
            image[i + 2] = gray;
        }
    }

    std::uint8_t* malloc_image(int size) {
        return static_cast<std::uint8_t*>(std::malloc(size));
    }

    void free_image(std::uint8_t* image) {
        std::free(image);
    }
}
