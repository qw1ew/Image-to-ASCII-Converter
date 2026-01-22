#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "ImageLoader.h"
#include "AsciiConverter.h"
#include "ConsoleUtils.h"
#include "GuiApp.h"

// Simple arg parser
struct Args {
    std::string inputPath;
    int width = 100; // Default width
    bool color = true;
    bool complexCharset = false;
};

Args parseArgs(int argc, char* argv[]) {
    Args args;
    if (argc < 2) {
        return args; // Invalid, but we check later
    }
    args.inputPath = argv[1];

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--no-color") {
            args.color = false;
        } else if (arg == "--complex") {
            args.complexCharset = true;
        } else {
            try {
                int w = std::stoi(arg);
                if (w > 0) args.width = w;
            } catch(...) {}
        }
    }
    return args;
}

int main(int argc, char* argv[]) {
    enableVirtualTerminal();

    Args args = parseArgs(argc, argv);
    
    // If no args (or just exe name), run GUI
    if (argc < 2) {
        GuiApp::run();
        return 0;
    }
    
    if (args.inputPath.empty()) {
        std::cerr << "Usage: ascii_converter <image_path> [width] [--no-color] [--complex]" << std::endl;
        return 1;
    }

    // Load
    ImageData img = ImageLoader::load(args.inputPath);
    if (img.pixels.empty()) {
        return 1;
    }

    // Calculate height based on aspect ratio
    // Font aspect ratio: chars are usually ~2x taller than wide.
    // So to keep aspect ratio, we should squash height by 0.5 (or duplicate width).
    // Effective Aspect Ratio = (Width / Height)
    // Target Aspect Ratio = (TargetWidth / TargetHeight) * (CharW / CharH)
    // We want Output AR == Image AR.
    // (TargetWidth / TargetHeight) * 0.5 = ImageWidth / ImageHeight
    // TargetHeight = TargetWidth * 0.5 * (ImageHeight / ImageWidth)
    
    float aspectRatio = (float)img.height / (float)img.width;
    int targetHeight = (int)(args.width * aspectRatio * 0.5f);
    if (targetHeight < 1) targetHeight = 1;

    // Resize
    ImageData resized = ImageLoader::resize(img, args.width, targetHeight);

    // Convert
    AsciiOptions opts;
    opts.color = args.color;
    if (args.complexCharset) {
        // More levels
        opts.customCharset = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. "; 
    } else {
        // Simpler levels for clearer shapes sometimes
        opts.customCharset = " .:-=+*#%@";
    }

    std::string ascii = AsciiConverter::convert(resized, opts);

    // Print
    std::cout << ascii;

    return 0;
}
