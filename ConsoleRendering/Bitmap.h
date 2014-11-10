#pragma once
#include <string>

struct RGB
{
    unsigned char r, g, b;
};

// Assumes 24 bit depth
class Bitmap
{
public:
    Bitmap() { }
    ~Bitmap() {
        if (bitmap != nullptr) { delete[] bitmap; }
    }

    bool Load24(std::string fileName);

    RGB GetPixel(int x, int y);

public:
    bool loaded = false;
    int width, height;
    short bpp;

private:
    RGB* bitmap = nullptr;
    std::string fileName;
};