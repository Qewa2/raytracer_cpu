#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP
// Disable strict warnings for this header from the Microsoft Visual C++
// compiler.
#ifdef _MSC_VER
#pragma warning(push, 0)
#endif

#include <string>

class imageLoader {
  public:
    imageLoader() {}

    imageLoader(const char *imageFilename);

    ~imageLoader();

    bool load(const std::string &filename);

    int width() const;
    int height() const;

    const unsigned char *pixelData(int x, int y) const;

  private:
    const int bytesPerPixel = 3;
    float *fdata = nullptr;         // Linear floating point pixel data
    unsigned char *bdata = nullptr; // Linear 8-bit pixel data
    int imageWidth = 0;             // Loaded image width
    int imageHeight = 0;            // Loaded image height
    int bytesPerScanline = 0;

    static int clamp(int x, int low, int high);

    static unsigned char floatToByte(float value);

    void convert_to_bytes();
};

// Restore MSVC compiler warnings
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // IMAGELOADER_HPP
