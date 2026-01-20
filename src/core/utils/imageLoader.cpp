#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include "dependencies/stb/stb_image.h"

#include "imageLoader.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

imageLoader::imageLoader(const char *imageFilename) {
    auto filename = std::string(imageFilename);
    auto imagedir = getenv("Images");

    if (imagedir && load(std::string(imagedir) + "/" + filename)) {
        return;
    } else {
        std::cerr << "ERROR: COuld not load image file '"
                  << std::string(imagedir) + "/" + filename << "'.\n";
        std::cerr << stbi_failure_reason() << "\n";
    }
}

imageLoader::~imageLoader() {
    delete[] bdata;
    STBI_FREE(fdata);
}

bool imageLoader::load(const std::string &filename) {
    // Loads the linear (gamma=1) image data from the given file name.
    // Returns true if the load succeeded. The resulting data buffer
    // contains the three [0.0, 1.0] floating-point values for the first
    // pixel (red, then green, then blue). Pixels are contiguous, going left
    // to right for the width of the image, followed by the next row below,
    // for the full height of the image.

    auto n = bytesPerPixel; // Dummy out parameter: original components
                            // per pixel
    fdata = stbi_loadf(filename.c_str(), &imageWidth, &imageHeight, &n,
                       bytesPerPixel);
    if (fdata == nullptr)
        return false;

    bytesPerScanline = imageWidth * bytesPerPixel;
    convert_to_bytes();
    return true;
}

int imageLoader::width() const { return (fdata == nullptr) ? 0 : imageWidth; }
int imageLoader::height() const { return (fdata == nullptr) ? 0 : imageHeight; }

const unsigned char *imageLoader::pixelData(int x, int y) const {
    // Return the address of the three RGB bytes of the pixel at x, y.
    // If there is no image datat it returns magenta
    static unsigned char magenta[] = {255, 0, 255};
    if (bdata == nullptr) {
        return magenta;
    }

    x = clamp(x, 0, imageWidth);
    y = clamp(y, 0, imageHeight);

    return bdata + y * bytesPerScanline + x * bytesPerPixel;
}

int imageLoader::clamp(int x, int low, int high) {
    if (x < low) {
        return low;
    }
    if (x > high) {
        return high - 1;
    }
    return x;
}

unsigned char imageLoader::floatToByte(float value) {
    if (value <= 0.0) {
        return 0;
    }
    if (1.0 <= value) {
        return 255;
    }
    return static_cast<unsigned char>(256.0 * value);
}

void imageLoader::convert_to_bytes() {
    // Convert the linear floating point pixel data to bytes, storing the
    // resulting byte data in the `bdata` member.

    int total_bytes = imageWidth * imageHeight * bytesPerPixel;
    bdata = new unsigned char[total_bytes];

    // Iterate through all pixel components, converting from [0.0, 1.0]
    // float values to unsigned [0, 255] byte values.

    auto *bptr = bdata;
    auto *fptr = fdata;
    for (auto i = 0; i < total_bytes; i++, fptr++, bptr++)
        *bptr = floatToByte(*fptr);
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
