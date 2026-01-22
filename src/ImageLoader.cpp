#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../include/stb_image_resize2.h"

#include "../include/libwebp/src/webp/decode.h"

#include "ImageLoader.h"
#include <iostream>
#include <fstream>
#include <algorithm>

ImageData ImageLoader::load(const std::string& path) {
    ImageData data;
    
    // Check extension
    std::string ext = "";
    size_t dot = path.find_last_of(".");
    if (dot != std::string::npos) {
        ext = path.substr(dot + 1);
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    }

    if (ext == "webp") {
        // Read file into memory
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            std::cerr << "Failed to open WebP file: " << path << std::endl;
            return data;
        }
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<uint8_t> buffer(size);
        if (!file.read((char*)buffer.data(), size)) {
            std::cerr << "Failed to read WebP file" << std::endl;
            return data;
        }

        int w, h;
        // Decode to RGB (3 channels)
        uint8_t* output = WebPDecodeRGB(buffer.data(), buffer.size(), &w, &h);
        if (!output) {
            std::cerr << "Failed to decode WebP data" << std::endl;
            return data;
        }

        data.width = w;
        data.height = h;
        data.channels = 3;
        data.pixels.assign(output, output + (w * h * 3));
        
        // WebPFree needed? Usually WebPDecodeRGB allocates with malloc, so free with free() or WebPFree.
        // Looking at headers, WebPFree is strictly for this.
        WebPFree(output);

        return data;
    }

    // Default to stbi for other formats
    int w, h, c;
    unsigned char* pixels = stbi_load(path.c_str(), &w, &h, &c, 3);
    
    if (!pixels) {
        std::cerr << "Failed to load image: " << path << std::endl;
        return data; 
    }

    data.width = w;
    data.height = h;
    data.channels = 3; 
    data.pixels.assign(pixels, pixels + (w * h * 3));

    stbi_image_free(pixels);
    return data;
}

ImageData ImageLoader::resize(const ImageData& original, int targetWidth, int targetHeight) {
    if (original.pixels.empty()) return {};

    ImageData resized;
    resized.width = targetWidth;
    resized.height = targetHeight;
    resized.channels = original.channels;
    resized.pixels.resize(targetWidth * targetHeight * original.channels);

    stbir_resize_uint8_linear(
        original.pixels.data(), original.width, original.height, 0,
        resized.pixels.data(), targetWidth, targetHeight, 0,
        (stbir_pixel_layout)original.channels 
    );

    return resized;
}
