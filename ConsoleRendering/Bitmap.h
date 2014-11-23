#pragma once
#include <string>
#include <map>

struct RGB
{
    unsigned char r, g, b;
};

class Bitmap
{
public:
    Bitmap() { }
    ~Bitmap() {
        if (bitmap != nullptr) { delete[] bitmap; }
    }

    // Load 24 bit depth bitmap
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