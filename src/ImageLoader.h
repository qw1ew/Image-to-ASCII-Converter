#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct ImageData {
    int width = 0;
    int height = 0;
    int channels = 0;
    std::vector<uint8_t> pixels;
};

class ImageLoader {
public:
    static ImageData load(const std::string& path);
    static ImageData resize(const ImageData& original, int targetWidth, int targetHeight);
};
