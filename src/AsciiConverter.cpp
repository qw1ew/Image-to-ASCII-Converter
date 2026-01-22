#include "AsciiConverter.h"
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>

// Standard "ramp" from dark to light
static const std::string DEFAULT_CHARSET = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

std::string AsciiConverter::convert(const ImageData& image, const AsciiOptions& options) {
    if (image.pixels.empty()) return "";

    std::stringstream ss;
    std::string charset = options.customCharset;
    if (charset.empty()) charset = DEFAULT_CHARSET;
    
    // If invert, reverse charset
    if (options.invert) {
        std::reverse(charset.begin(), charset.end());
    }

    int w = image.width;
    int h = image.height;
    int c = image.channels;
    const auto& px = image.pixels;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int idx = (y * w + x) * c;
            uint8_t r = px[idx];
            uint8_t g = px[idx + 1];
            uint8_t b = px[idx + 2];

            // Luminance standard
            float lum = 0.2126f * r + 0.7152f * g + 0.0722f * b;
            
            // Map 0-255 to 0-(charset.len-1)
            int charIndex = (int)((lum / 255.0f) * (charset.length() - 1));
            // Clamp just in case
            if (charIndex < 0) charIndex = 0;
            if (charIndex >= (int)charset.length()) charIndex = charset.length() - 1;

            if (options.color) {
                // ANSI 24-bit color: \x1b[38;2;R;G;Bm
                // We also need to reset color maybe, or just keep setting it.
                // Setting it per char is expensive but accurate.
                ss << "\x1b[38;2;" << (int)r << ";" << (int)g << ";" << (int)b << "m";
            }
            
            ss << charset[charIndex];
        }
        if (options.color) {
            ss << "\x1b[0m"; // Reset color at end of line
        }
        ss << "\n";
    }

    return ss.str();
}
