#pragma once
#include "ImageLoader.h"
#include <string>

struct AsciiOptions {
    bool color = true;
    bool invert = false;
    std::string customCharset = ""; // If empty, use default
};

class AsciiConverter {
public:
    static std::string convert(const ImageData& image, const AsciiOptions& options);
};
